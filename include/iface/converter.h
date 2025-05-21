#pragma once

#include "iface.h"
#include "system.h"
#include <ginac/ginac.h>
#include <QString>

namespace mpp {

struct IConverter: Iface
{
    virtual GiNaC::ex convertToEx(std::string equation) const = 0;
    virtual std::string convertToString(const GiNaC::ex& equation) const  = 0;
    virtual GiNaC::ex convertToEx(const QString& equation) const = 0;
    virtual QString convertToQString(const GiNaC::ex& equation) const = 0;
    virtual QString convertToQstring(const std::vector<double>& val) const = 0;

    virtual mpp::ISystem* convertToMppModel(gui::ISystem* system) const = 0;
    virtual GiNaC::exmap convertToExmap(const std::vector<double>& point) const = 0;
    virtual GiNaC::exmap convertToExmap2(const std::vector<double>& point) const = 0;
    virtual std::vector<double> convertToPointType(GiNaC::exmap pointMap, unsigned int dim) const = 0;
};

} // namespace mpp
