#pragma once

#include <string>
#include "VehicleConfig.h"

namespace Vehicle
{
    /// <summary>
    /// Loads a VehicleConfig from a JSON file via nlohmann/json
    /// </summary>
    class VehicleConfigLoader
    {
    public:
        /// Returns false and fills 'outError' on a missing file or malformed JSON.
        /// Validation failures also return false.
        static bool LoadFromFile(const std::string& path,
            VehicleConfig& outConfig,
            std::string& outError);

        /// Same, but takes the JSON text directly. Useful for tests.
        static bool LoadFromString(const std::string& json,
            VehicleConfig& outConfig,
            std::string& outError);

        /// Writes the config back out, so you can dump the built-in default and
        /// use it as a starting point for tuning.
        static bool SaveToFile(const std::string& path,
            const VehicleConfig& config,
            std::string& outError);
    };
}
