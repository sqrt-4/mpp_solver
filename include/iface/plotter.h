#pragma once

#include "iface.h"
#include <vector>
#include <string>

struct IPlotter: Iface
{
    // returns relative to bin path to created .png
    virtual std::string createPng(const std::string& axis1, const std::string& axis2) const = 0;
};
