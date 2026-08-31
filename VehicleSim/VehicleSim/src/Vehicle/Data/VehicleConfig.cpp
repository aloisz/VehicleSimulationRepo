#include "VehicleConfig.h"

#include "../../Core/Log.h"

#include <algorithm>
#include <cmath>

namespace Vehicle
{
    const char* ToString(EDrivetrainType type)
    {
        switch (type)
        {
            case EDrivetrainType::FWD: 
                return "FWD";

            case EDrivetrainType::AWD: 
                return "AWD";

            default:
                return "RWD";
        }
    }

    bool ParseDrivetrainType(const std::string& text, EDrivetrainType& out)
    {
        std::string upper;
        upper.reserve(text.size());

        for (char c : text) 
            upper.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));

        if (upper == "FWD") 
        { 
            out = EDrivetrainType::FWD; 
            return true; 
        }

        if (upper == "RWD")
        {
            out = EDrivetrainType::RWD; 
            return true; 
        }

        if (upper == "AWD" || upper == "4WD") 
        { 
            out = EDrivetrainType::AWD; 
            return true; 
        }

        return false;
    }

    bool VehicleConfig::Validate()
    {
        if (Wheels.empty())
        {
            Log::Error("VehicleConfig '" + Name + "' has no wheels", Log::Category::Config);
            return false;
        }

        if (Mass <= 1.0f)
        {
            Log::Error("VehicleConfig '" + Name + "' has an invalid mass", Log::Category::Config);
            return false;
        }

        if (Gearbox.Ratios.empty())
        {
            Log::Warning("No gear ratios given, falling back to a single 1:1 gear", Log::Category::Config);
            Gearbox.Ratios = { 1.0f };
        }

        // The differential type is authoritative over the per-wheel IsDriven flag
        int maxAxle = 0;
        for (const WheelConfig& w : Wheels)
            maxAxle = std::max(maxAxle, w.Axle);

        for (WheelConfig& w : Wheels)
        {
            switch (Differential.Type)
            {
            case EDrivetrainType::FWD: 
                w.IsDriven = (w.Axle == 0); 
                break;

            case EDrivetrainType::RWD: 
                w.IsDriven = (w.Axle == maxAxle); 
                break;

            case EDrivetrainType::AWD: 
                w.IsDriven = true; 
                break;
            }
        }

        const float gravity = 9.81f;
        const float nominalLoad = Mass * gravity / static_cast<float>(Wheels.size());

        for (WheelConfig& w : Wheels)
        {
            if (w.Radius <= 0.01f)
            {
                Log::Error("Wheel '" + w.Name + "' has an invalid radius", Log::Category::Config);
                return false;
            }

            if (w.Suspension.Length <= 0.0f)
            {
                Log::Error("Wheel '" + w.Name + "' has an invalid suspension length", Log::Category::Config);
                return false;
            }

            w.Suspension.RestLength = 
                MathUtils::Clamp(w.Suspension.RestLength, 0.02f, w.Suspension.Length * 0.95f);

            const float restTravel = w.Suspension.Length - w.Suspension.RestLength;

            // Auto derive the spring rate so the car sits exactly at RestLength
            if (w.Suspension.SpringStrength <= 0.0f)
            {
                w.Suspension.SpringStrength = nominalLoad / restTravel;
                Log::Info("Wheel '" + w.Name + "': derived spring rate " +
                    std::to_string(static_cast<int>(w.Suspension.SpringStrength)) + " N/m",
                    Log::Category::Config);
            }

            if (w.Suspension.DamperStrength <= 0.0f)
            {
                const float cornerMass = Mass / static_cast<float>(Wheels.size());
                w.Suspension.DamperStrength =
                    0.85f * 2.0f * std::sqrt(w.Suspension.SpringStrength * cornerMass);
            }

            const float sag = nominalLoad / w.Suspension.SpringStrength;
            if (sag > w.Suspension.Length * 0.9f)
            {
                Log::Warning("Wheel '" + w.Name + "' springs are too soft: the suspension "
                    "bottoms out under static load", Log::Category::Config);
            }

            if (w.Suspension.MaxForce < nominalLoad * 2.0f)
            {
                Log::Warning("Wheel '" + w.Name + "' suspension MaxForce is low relative to "
                    "static load; the car will feel like it is falling through bumps",
                    Log::Category::Config);
            }

            if (w.Tire.PeakSlipRatio <= 1e-4f) w.Tire.PeakSlipRatio = 0.12f;
            if (w.Tire.PeakSlipAngle <= 1e-4f) w.Tire.PeakSlipAngle = 0.14f;
        }

        bool anyDriven = false;
        bool anySteerable = false;
        for (const WheelConfig& w : Wheels)
        {
            anyDriven |= w.IsDriven;
            anySteerable |= w.IsSteerable;
        }

        if (!anyDriven)
        {
            Log::Error("No driven wheel resolved for drivetrain type " +
                std::string(ToString(Differential.Type)), Log::Category::Config);
            return false;
        }

        if (!anySteerable)
            Log::Warning("No steerable wheel: the vehicle will not turn", Log::Category::Config);

        if (Engine.PeakTorqueRPM <= Engine.IdleRPM || Engine.PeakTorqueRPM >= Engine.MaxRPM)
        {
            Engine.PeakTorqueRPM = MathUtils::Lerp(Engine.IdleRPM, Engine.MaxRPM, 0.55f);
            Log::Warning("PeakTorqueRPM out of range, clamped inside the rev band",
                Log::Category::Config);
        }

        Differential.FrontBias = MathUtils::Clamp01(Differential.FrontBias);
        Brakes.FrontBias = MathUtils::Clamp01(Brakes.FrontBias);
        Gearbox.Efficiency = MathUtils::Clamp(Gearbox.Efficiency, 0.1f, 1.0f);

        return true;
    }

    VehicleConfig VehicleConfig::MakeDefault()
    {
        VehicleConfig cfg;
        cfg.Name = "Default Vehicle Simulation";

        const float halfTrack = 0.78f;
        const float wheelbaseHalf = 1.35f;
        const float axleY = -0.25f;
            
        auto makeWheel = [&](const char* name, float x, float z, int axle, bool steer)
            {
                WheelConfig w;
                w.Name = name;
                w.LocalOffset = btVector3(x, axleY, z);
                w.Axle = axle;
                w.IsSteerable = steer;
                return w;
            };

        cfg.Wheels.push_back(makeWheel("FL", -halfTrack, wheelbaseHalf, 0, true));
        cfg.Wheels.push_back(makeWheel("FR", halfTrack, wheelbaseHalf, 0, true));
        cfg.Wheels.push_back(makeWheel("RL", -halfTrack, -wheelbaseHalf, 1, false));
        cfg.Wheels.push_back(makeWheel("RR", halfTrack, -wheelbaseHalf, 1, false));

        cfg.Validate();
        return cfg;
    }
}
