#pragma once

#include "IVehicleModule.h"
#include <vector>

namespace Vehicle
{
    /// <summary>
    /// handles gearbox, clutch and torque split
    ///   UpdateGearbox()    - automatic or manual gear selection, shift timing
    ///   UpdateClutch()     - clutch lock as a function of RPM and shift state
    ///   DistributeTorque() - engine torque -> per-wheel drive torque
    /// </summary>
    class VehicleDrivetrainModule : public IVehicleModule
    {
    public:
        bool Initialize(VehicleContext& context) override;
        void Update(float dt, VehicleContext& context) override;
        void Dispose() override;

        const char* GetName() const override { return "VehicleDrivetrainModule"; }

    private:
        void UpdateWheelFeedback(VehicleContext& context);
        void UpdateGearbox(float dt, VehicleContext& context);
        bool ShiftingInProgress(float dt, VehicleRuntime& runtime, const GearboxConfig& gearbox);
        bool ManualShifting(float dt, VehicleContext& context, VehicleRuntime& runtime);

        void UpdateClutch(float dt, VehicleContext& context);
        void DistributeTorque(VehicleContext& context);

        void RequestGear(int gear, VehicleContext& context);

        static float GetTotalRatio(const GearboxConfig& gearbox, int gear);

        /// <summary>
        /// Indices into Config->Wheels
        /// </summary>
        std::vector<size_t> _drivenWheels;
        std::vector<size_t> _drivenFront;
        std::vector<size_t> _drivenRear;

        int _maxAxle = 0;
        int _maxGear = 0;

        /// <summary>
        /// in m/s
        /// </summary>
        static constexpr float REVERSE_ENGAGE_SPEED = 0.8f;
    };
}
