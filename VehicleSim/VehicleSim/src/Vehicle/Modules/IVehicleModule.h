#pragma once

#include "../Data/VehicleContext.h"

namespace Vehicle
{
    /// <summary>
    /// Initialize vehicle modules
    /// </summary>
    class IVehicleModule
    {
    public:
        virtual ~IVehicleModule() = default;

        /// Returns false if the module cannot run with this configuration
        virtual bool Initialize(VehicleContext& context) = 0;

        virtual void Update(float dt, VehicleContext& context) = 0;
        virtual void Dispose() {}

        virtual const char* GetName() const = 0;

    protected:
        bool _initialized = false;
    };
}