#include "VehicleConfigLoader.h"

#include "../../Core/Log.h"
#include "../../Core/MathUtils.h"

#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>

using json = nlohmann::json;

namespace
{
    void ReadFloat(const json& j, const char* key, float& dst)
    {
        if (j.contains(key) && j[key].is_number())
            dst = j[key].get<float>();
    }

    void ReadDegreesAsRadians(const json& j, const char* key, float& dst)
    {
        if (j.contains(key) && j[key].is_number())
            dst = j[key].get<float>() * MathUtils::DEG2RAD;
    }

    void ReadBool(const json& j, const char* key, bool& dst)
    {
        if (j.contains(key) && j[key].is_boolean())
            dst = j[key].get<bool>();
    }

    void ReadInt(const json& j, const char* key, int& dst)
    {
        if (j.contains(key) && j[key].is_number_integer())
            dst = j[key].get<int>();
    }

    void ReadString(const json& j, const char* key, std::string& dst)
    {
        if (j.contains(key) && j[key].is_string())
            dst = j[key].get<std::string>();
    }

    void ReadChar(const json& j, const char* key, char& dst)
    {
        if (!j.contains(key)) return;

        const json& v = j[key];

        if (v.is_string())
        {
            const std::string& str = v.get<std::string>();
            if (!str.empty())
                dst = str[0];
        }
        else if (v.is_number_integer())
        {
            const int code = v.get<int>();
            if (code > 0 && code < 128)
                dst = static_cast<char>(code);
            else
                Log::Warning(std::string("Key code out of range for '") + key + "'",
                    Log::Category::Config);
        }
    }

    void ReadVec3(const json& j, const char* key, btVector3& dst)
    {
        if (!j.contains(key)) return;

        const json& v = j[key];
        if (v.is_array() && v.size() == 3)
        {
            dst = btVector3(v[0].get<float>(), v[1].get<float>(), v[2].get<float>());
        }
        else if (v.is_object())
        {
            float x = dst.getX(), y = dst.getY(), z = dst.getZ();
            ReadFloat(v, "x", x);
            ReadFloat(v, "y", y);
            ReadFloat(v, "z", z);
            dst = btVector3(x, y, z);
        }
    }

    json WriteVec3(const btVector3& v)
    {
        return json::array({ v.getX(), v.getY(), v.getZ() });
    }

    // === Section readers ===

    void ReadSuspension(const json& j, Vehicle::SuspensionConfig& s)
    {
        ReadFloat(j, "length", s.Length);
        ReadFloat(j, "springStrength", s.SpringStrength);
        ReadFloat(j, "damperStrength", s.DamperStrength);
        ReadFloat(j, "maxForce", s.MaxForce);
        ReadFloat(j, "restLength", s.RestLength);
    }

    void ReadTire(const json& j, Vehicle::TireConfig& t)
    {
        ReadFloat(j, "peakSlipRatio", t.PeakSlipRatio);
        ReadFloat(j, "peakSlipAngle", t.PeakSlipAngle); // radians
        ReadDegreesAsRadians(j, "peakSlipAngleDeg", t.PeakSlipAngle);
        ReadFloat(j, "longGrip", t.LongGrip);
        ReadFloat(j, "latGrip", t.LatGrip);
        ReadFloat(j, "frictionCoefficient", t.FrictionCoefficient);
        ReadFloat(j, "loadSensitivity", t.LoadSensitivity);
        ReadFloat(j, "rollingResistance", t.RollingResistance);
    }

    void ReadWheel(const json& j, Vehicle::WheelConfig& w)
    {
        ReadString(j, "name", w.Name);
        ReadVec3(j, "localOffset", w.LocalOffset);
        ReadFloat(j, "radius", w.Radius);
        ReadFloat(j, "width", w.Width);
        ReadFloat(j, "mass", w.Mass);
        ReadInt(j, "axle", w.Axle);
        ReadBool(j, "isDriven", w.IsDriven);
        ReadBool(j, "isSteerable", w.IsSteerable);

        if (j.contains("suspension")) ReadSuspension(j["suspension"], w.Suspension);
        if (j.contains("tire"))       ReadTire(j["tire"], w.Tire);
    }

    void ReadEngine(const json& j, Vehicle::EngineConfig& e)
    {
        ReadFloat(j, "idleRPM", e.IdleRPM);
        ReadFloat(j, "maxRPM", e.MaxRPM);
        ReadFloat(j, "peakTorqueRPM", e.PeakTorqueRPM);
        ReadFloat(j, "maxTorque", e.MaxTorque);
        ReadFloat(j, "inertia", e.Inertia);
        ReadFloat(j, "engineBrakeTorque", e.EngineBrakeTorque);
    }

    void ReadGearbox(const json& j, Vehicle::GearboxConfig& g)
    {
        if (j.contains("ratios") && j["ratios"].is_array())
        {
            g.Ratios.clear();
            for (const auto& r : j["ratios"])
                if (r.is_number()) g.Ratios.push_back(r.get<float>());
        }

        ReadFloat(j, "finalDrive", g.FinalDrive);
        ReadFloat(j, "reverseRatio", g.ReverseRatio);
        ReadFloat(j, "efficiency", g.Efficiency);
        ReadFloat(j, "shiftTime", g.ShiftTime);
        ReadFloat(j, "upshiftRPM", g.UpshiftRPM);
        ReadFloat(j, "downshiftRPM", g.DownshiftRPM);
        ReadFloat(j, "autoReverseThrottleThreshold", g.AutoReverseThrottleThreshold);
        ReadFloat(j, "autoReverseBrakeThreshold", g.AutoReverseBrakeThreshold);
        ReadFloat(j, "clutchEngageRPM", g.ClutchEngageRPM);
    }

    void ReadDifferential(const json& j, Vehicle::DifferentialConfig& d)
    {
        std::string type;
        ReadString(j, "type", type);
        if (!type.empty() && !Vehicle::ParseDrivetrainType(type, d.Type))
            Log::Warning("Unknown drivetrain type '" + type + "', keeping " +
                Vehicle::ToString(d.Type), Log::Category::Config);

        ReadFloat(j, "frontBias", d.FrontBias);
    }

    void ReadAero(const json& j, Vehicle::AerodynamicsConfig& a)
    {
        ReadFloat(j, "airDensity", a.AirDensity);
        ReadFloat(j, "dragCoefficient", a.DragCoefficient);
        ReadFloat(j, "frontalArea", a.FrontalArea);
        ReadFloat(j, "downforceCoefficient", a.DownforceCoefficient);
        ReadVec3(j, "centerOfPressureOffset", a.CenterOfPressureOffset);
    }

    void ReadSteering(const json& j, Vehicle::SteeringConfig& s)
    {
        ReadFloat(j, "maxSteerAngle", s.MaxSteerAngle);
        ReadDegreesAsRadians(j, "maxSteerAngleDeg", s.MaxSteerAngle);
        ReadFloat(j, "speedSensitivity", s.SpeedSensitivity);
        ReadDegreesAsRadians(j, "steerRateDegPerSec", s.SteerRate);
        ReadDegreesAsRadians(j, "returnRateDegPerSec", s.ReturnRate);
    }

    void ReadInput(const json& j, Vehicle::InputConfig& i)
    {
        ReadChar(j, "throttleInput", i.throttleInput);
        ReadChar(j, "brakeInput", i.brakeInput);
        ReadChar(j, "steerLeftInput", i.steerLeftInput);
        ReadChar(j, "steerRightInput", i.steerRightInput);
        ReadChar(j, "gearShiftUpInput", i.gearShiftUpInput);
        ReadChar(j, "gearShiftDownInput", i.gearShiftDownInput);
        ReadChar(j, "changeGearboxType", i.changeGearboxType);
    }

    void ReadBrakes(const json& j, Vehicle::BrakeConfig& b)
    {
        ReadFloat(j, "maxBrakeTorque", b.MaxBrakeTorque);
        ReadFloat(j, "frontBias", b.FrontBias);
        ReadFloat(j, "handbrakeTorque", b.HandbrakeTorque);
        ReadBool(j, "absEnabled", b.AbsEnabled);
        ReadFloat(j, "absSlipThreshold", b.AbsSlipThreshold);
        ReadBool(j, "tractionControlEnabled", b.TractionControlEnabled);
        ReadFloat(j, "tcSlipThreshold", b.TcSlipThreshold);
    }

    void ReadAntiRoll(const json& j, Vehicle::AntiRollConfig& a)
    {
        ReadFloat(j, "frontStiffness", a.FrontStiffness);
        ReadFloat(j, "rearStiffness", a.RearStiffness);
    }
}

namespace Vehicle
{
    bool VehicleConfigLoader::LoadFromString(const std::string& text,
        VehicleConfig& cfg,
        std::string& outError)
    {
        json root;
        try
        {
            root = json::parse(text, nullptr, true, /*ignore_comments*/ true);
        }
        catch (const json::exception& e)
        {
            outError = std::string("JSON parse error: ") + e.what();
            return false;
        }

        if (!root.is_object())
        {
            outError = "Root of the vehicle config must be a JSON object";
            return false;
        }

        try
        {
            ReadString(root, "name", cfg.Name);
            ReadFloat(root, "mass", cfg.Mass);
            ReadVec3(root, "bodySize", cfg.BodySize);
            ReadVec3(root, "centerOfMassOffset", cfg.CenterOfMassOffset);

            if (root.contains("engine"))
            {
                ReadEngine(root["engine"], cfg.Engine);
            }

            if (root.contains("gearbox")) 
            {
                ReadGearbox(root["gearbox"], cfg.Gearbox);
            }

            if (root.contains("differential")) 
            {
                ReadDifferential(root["differential"], cfg.Differential);
            }

            if (root.contains("aerodynamics")) 
            {
                ReadAero(root["aerodynamics"], cfg.Aero);
            }

            if (root.contains("steering")) 
            {
                ReadSteering(root["steering"], cfg.Steering);
            }

            if (root.contains("input"))
            {
                ReadInput(root["input"], cfg.Input);
            }

            if (root.contains("brakes")) 
            {
                ReadBrakes(root["brakes"], cfg.Brakes);
            }

            if (root.contains("antiRoll"))
            {
                ReadAntiRoll(root["antiRoll"], cfg.AntiRoll);
            }

            if (root.contains("wheels") && root["wheels"].is_array())
            {
                cfg.Wheels.clear();
                for (const auto& jw : root["wheels"])
                {
                    WheelConfig w;
                    ReadWheel(jw, w);
                    cfg.Wheels.push_back(w);
                }
            }
        }
        catch (const json::exception& e)
        {
            outError = std::string("JSON field error: ") + e.what();
            return false;
        }

        if (!cfg.Validate())
        {
            outError = "Vehicle config failed validation (see log above)";
            return false;
        }

        return true;
    }

    bool VehicleConfigLoader::LoadFromFile(const std::string& path,
        VehicleConfig& cfg,
        std::string& outError)
    {
        std::ifstream file(path);
        if (!file.is_open())
        {
            outError = "Could not open vehicle config '" + path + "'";
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();

        if (!LoadFromString(buffer.str(), cfg, outError))
            return false;

        Log::Info("Loaded vehicle config '" + cfg.Name + "' from " + path, Log::Category::Config);
        return true;
    }

    bool VehicleConfigLoader::SaveToFile(const std::string& path,
        const VehicleConfig& cfg,
        std::string& outError)
    {
        json root;
        root["name"] = cfg.Name;
        root["mass"] = cfg.Mass;
        root["bodySize"] = WriteVec3(cfg.BodySize);
        root["centerOfMassOffset"] = WriteVec3(cfg.CenterOfMassOffset);

        root["engine"] = {
            { "idleRPM", cfg.Engine.IdleRPM },
            { "maxRPM", cfg.Engine.MaxRPM },
            { "peakTorqueRPM", cfg.Engine.PeakTorqueRPM },
            { "maxTorque", cfg.Engine.MaxTorque },
            { "inertia", cfg.Engine.Inertia },
            { "engineBrakeTorque", cfg.Engine.EngineBrakeTorque }
        };

        root["gearbox"] = {
            { "ratios", cfg.Gearbox.Ratios },
            { "finalDrive", cfg.Gearbox.FinalDrive },
            { "reverseRatio", cfg.Gearbox.ReverseRatio },
            { "efficiency", cfg.Gearbox.Efficiency },
            { "shiftTime", cfg.Gearbox.ShiftTime },
            { "upshiftRPM", cfg.Gearbox.UpshiftRPM },
            { "downshiftRPM", cfg.Gearbox.DownshiftRPM },
            { "autoReverseThrottleThreshold", cfg.Gearbox.AutoReverseThrottleThreshold },
            { "autoReverseBrakeThreshold", cfg.Gearbox.AutoReverseBrakeThreshold },
            { "clutchEngageRPM", cfg.Gearbox.ClutchEngageRPM }
        };

        root["differential"] = {
            { "type", ToString(cfg.Differential.Type) },
            { "frontBias", cfg.Differential.FrontBias }
        };

        root["aerodynamics"] = {
            { "airDensity", cfg.Aero.AirDensity },
            { "dragCoefficient", cfg.Aero.DragCoefficient },
            { "frontalArea", cfg.Aero.FrontalArea },
            { "downforceCoefficient", cfg.Aero.DownforceCoefficient },
            { "centerOfPressureOffset", WriteVec3(cfg.Aero.CenterOfPressureOffset) }
        };

        root["steering"] = {
            { "maxSteerAngleDeg", cfg.Steering.MaxSteerAngle * MathUtils::RAD2DEG },
            { "speedSensitivity", cfg.Steering.SpeedSensitivity },
            { "steerRateDegPerSec", cfg.Steering.SteerRate * MathUtils::RAD2DEG },
            { "returnRateDegPerSec", cfg.Steering.ReturnRate * MathUtils::RAD2DEG }
        };

        auto KeyToJson = [](char c) { return std::string(1, c); };

        root["input"] = {
            { "throttleInput",KeyToJson(cfg.Input.throttleInput) },
            { "brakeInput", KeyToJson(cfg.Input.brakeInput) },
            { "steerLeftInput", KeyToJson(cfg.Input.steerLeftInput) },
            { "steerRightInput", KeyToJson(cfg.Input.steerRightInput) },
            { "gearShiftUpInput", KeyToJson(cfg.Input.gearShiftUpInput) },
            { "gearShiftDownInput", KeyToJson(cfg.Input.gearShiftDownInput) },
            { "changeGearboxType", KeyToJson(cfg.Input.changeGearboxType) }
        };

        root["brakes"] = {
            { "maxBrakeTorque", cfg.Brakes.MaxBrakeTorque },
            { "frontBias", cfg.Brakes.FrontBias },
            { "handbrakeTorque", cfg.Brakes.HandbrakeTorque },
            { "absEnabled", cfg.Brakes.AbsEnabled },
            { "absSlipThreshold", cfg.Brakes.AbsSlipThreshold },
            { "tractionControlEnabled", cfg.Brakes.TractionControlEnabled },
            { "tcSlipThreshold", cfg.Brakes.TcSlipThreshold }
        };

        root["antiRoll"] = {
            { "frontStiffness", cfg.AntiRoll.FrontStiffness },
            { "rearStiffness", cfg.AntiRoll.RearStiffness }
        };

        json wheels = json::array();
        for (const WheelConfig& w : cfg.Wheels)
        {
            wheels.push_back({
                { "name", w.Name },
                { "localOffset", WriteVec3(w.LocalOffset) },
                { "radius", w.Radius },
                { "width", w.Width },
                { "mass", w.Mass },
                { "axle", w.Axle },
                { "isSteerable", w.IsSteerable },
                { "suspension", {
                    { "length", w.Suspension.Length },
                    { "springStrength", w.Suspension.SpringStrength },
                    { "damperStrength", w.Suspension.DamperStrength },
                    { "maxForce", w.Suspension.MaxForce },
                    { "restLength", w.Suspension.RestLength }
                }},
                { "tire", {
                    { "peakSlipRatio", w.Tire.PeakSlipRatio },
                    { "peakSlipAngleDeg", w.Tire.PeakSlipAngle * MathUtils::RAD2DEG },
                    { "longGrip", w.Tire.LongGrip },
                    { "latGrip", w.Tire.LatGrip },
                    { "frictionCoefficient", w.Tire.FrictionCoefficient },
                    { "loadSensitivity", w.Tire.LoadSensitivity },
                    { "rollingResistance", w.Tire.RollingResistance }
                }}
                });
        }
        root["wheels"] = wheels;

        std::ofstream out(path);
        if (!out.is_open())
        {
            outError = "Could not write vehicle config to '" + path + "'";
            return false;
        }

        out << root.dump(4) << std::endl;
        return true;
    }
}
