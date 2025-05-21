#pragma once

#include <iface/plotter.h>

namespace mpp {

class Plotter: public IPlotter
{
protected:
    virtual std::string createPng(const std::string& axis1, const std::string& axis2) const override;
};

} // namespace mpp
