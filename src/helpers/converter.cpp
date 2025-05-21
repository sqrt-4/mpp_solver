#include <helpers/converter.h>
#include <mpp/systemModel.h>
#include <gui/systemModel.h>
#include <symbols.h>
#include <sstream>
#include <memory.h>

namespace mpp {

namespace {

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', advance start_pos to avoid infinite loop
    }
}

std::vector<double> parseStartPoint(std::stringstream& ss)
{
    std::vector<double> startPoint;
    float tmp;
    while (ss >> tmp)
    {
        startPoint.push_back(tmp);
    }
    return startPoint;
}

} // namespace

GiNaC::ex Converter::convertToEx(std::string equation) const
{
    GiNaC::lst l;
    for (size_t i = 1; i < Symbols::SYM.size(); ++i)
    {
        std::string pattern("x" + std::to_string(i));
        replaceAll(equation, pattern, Symbols::SYM[i].get_name());
        l.append(Symbols::SYM[i]);
    }

    return { equation, l };
}

GiNaC::ex Converter::convertToEx(const QString& equation) const
{
    return convertToEx(equation.toStdString());
}

std::string Converter::convertToString(const GiNaC::ex& equation) const
{
    std::stringstream ss;
    ss << equation;
    return ss.str();
}

QString Converter::convertToQString(const GiNaC::ex& equation) const
{
    return QString::fromStdString(convertToString(equation));
}

QString Converter::convertToQstring(const std::vector<double>& val) const
{
    std::stringstream ss;
    for (const auto& it: val)
    {
        ss << it << ' ';
    }
    return QString::fromStdString(ss.str());
}

std::vector<double> Converter::convertToPointType(GiNaC::exmap pointMap, unsigned int dim) const
{
    std::vector<double> res;
    for (size_t i = 0; i < dim; ++i)
    {
        const auto val = GiNaC::ex_to<GiNaC::numeric>(pointMap[Symbols::SYM[i+1]]).to_double();
        res.push_back(val);
    }
    return res;
}

mpp::ISystem* Converter::convertToMppModel(gui::ISystem* system) const
{
    std::unique_ptr<mpp::ISystem> resSys = std::make_unique<mpp::System>();

    const auto equations = system->getEquations();
    const auto R = system->getR();
    const auto P0 = system->getP0();

    if (equations.size() != R.size())
    {
        throw std::runtime_error("equations and R are not the same size");
    }

    for (const auto& eq: equations)
    {
        resSys->addEquation(convertToEx(eq));
    }
    for (const auto& r: R)
    {
        std::string eq = r.toStdString();
        const auto n = resSys->getDim();
        for (size_t i = 1; i <= resSys->getDim(); ++i)
        {
            std::string pattern("x" + std::to_string(i) + "(a)");
            replaceAll(eq, pattern, "x" + std::to_string(i));
            pattern = "x" + std::to_string(i) + "(b)";
            replaceAll(eq, pattern, "x" + std::to_string(n+i));
        }
        QString changedR(eq.c_str());
        std::cout << "R: " << eq << "\n";
        resSys->addR(convertToEx(changedR));
    }
    for (const auto& p0: P0)
    {
        resSys->addP0(p0);
    }

    resSys->setTa(system->getTa());
    resSys->setTb(system->getTb());
    resSys->setTs(system->getTs());

    return resSys.release();
}

GiNaC::exmap Converter::convertToExmap(const std::vector<double>& point) const
{
    GiNaC::exmap res;
    for (size_t i = 0; i < point.size(); ++i)
    {
        res[Symbols::SYM[i+1]] = GiNaC::ex(point[i]);
    }
    return res;
}

GiNaC::exmap Converter::convertToExmap2(const std::vector<double>& point) const
{
    GiNaC::exmap res;
    int n = point.size() / 2;
    for (size_t i = 0; i < n; i++)
    {
        res[Symbols::SYM[i + 1]] = GiNaC::ex(point[i]);
        res[Symbols::SYM[n + i + 1]] = GiNaC::ex(point[n+i]);
    }
    return res;
}

} // namespace mpp
