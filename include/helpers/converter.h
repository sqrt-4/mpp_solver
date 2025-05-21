#pragma once

#include <iface/converter.h>
#include <ginac/ginac.h>
#include <QString>

namespace mpp {

class Converter: public IConverter
{
private:
    GiNaC::ex convertToEx(std::string equation) const override;
    std::string convertToString(const GiNaC::ex& equation) const override;
    GiNaC::ex convertToEx(const QString& equation) const override;
    QString convertToQString(const GiNaC::ex& equation) const override;
    QString convertToQstring(const std::vector<double>& val) const override;

    mpp::ISystem* convertToMppModel(gui::ISystem* system) const override;
    GiNaC::exmap convertToExmap(const std::vector<double>& point) const override;
    GiNaC::exmap convertToExmap2(const std::vector<double>& point) const override;
    std::vector<double> convertToPointType(GiNaC::exmap pointMap, unsigned int dim) const override;
};

} // namespace mpp
