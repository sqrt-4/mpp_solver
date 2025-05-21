#pragma once

#include "iface.h"
#include "system.h"
#include <ginac/ginac.h>

namespace mpp {

using PointType = std::vector<double>;

struct IEngine: Iface
{
    virtual std::vector<PointType> compute_solution(ISystem* system) = 0;
};

} // namespace mpp
