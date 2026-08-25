#pragma once

#include <iostream>
#include <string>
#include "Color.h"

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

    template <typename... Args>
    std::string Format(Args&&... args)
    {
        std::ostringstream ss;
        (ss << ... << std::forward<Args>(args));
        return ss.str();
    }

    inline void Info(const std::string& msg, Category c = Category::General)
    {
        std::cout << Color::Gray << "[INFO]"
            << Color::Cyan << "[" << ToString(c) << "] "
            << Color::White << msg << Color::Reset << std::endl;
    }

    inline void Warning(const std::string& msg, Category c = Category::General)
    {
        std::cout << Color::Yellow << "[WARNING][" << ToString(c) << "] "
            << msg << Color::Reset << std::endl;
    }

    inline void Error(const std::string& msg, Category c = Category::General)
    {
        std::cerr << Color::Red << "[ERROR][" << ToString(c) << "] "
            << msg << Color::Reset << std::endl;
    }
}
