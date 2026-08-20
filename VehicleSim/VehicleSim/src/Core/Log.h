#pragma once

#include <iostream>
#include <string>

/// <summary>
/// Log helper
/// </summary>
namespace Log
{
    enum class Category
    {
        General,
        Physics,
        Vehicle,
        Config,
        Render
    };

    inline const char* ToString(Category c)
    {
        switch (c)
        {
            case Category::Physics: 
                return "Physics";

            case Category::Vehicle: 
                return "Vehicle";
            case Category::Config:  
                return "Config";

            case Category::Render:  
                return "Render";

            default:
                return "General";
        }
    }

    inline void Info(const std::string& msg, Category c = Category::General)
    {
        std::cout << "[INFO ][" << ToString(c) << "] " << msg << std::endl;
    }

    inline void Warning(const std::string& msg, Category c = Category::General)
    {
        std::cout << "[WARNING ][" << ToString(c) << "] " << msg << std::endl;
    }

    inline void Error(const std::string& msg, Category c = Category::General)
    {
        std::cerr << "[ERROR][" << ToString(c) << "] " << msg << std::endl;
    }
}
