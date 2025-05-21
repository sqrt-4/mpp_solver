#include <mpp/systemModel.h>
#include <iface/converter.h>
#include <helpers/converter.h>
#include <symbols.h>
#include <memory>

namespace mpp {

System::System(uint dim, const std::vector<std::string>& equations, const std::vector<std::string>& R) : m_dim(dim)
{
    assert(dim == equations.size());
    assert(dim == R.size());
    std::unique_ptr<IConverter> conv(new Converter);
    for (size_t i = 0; i < dim; i++)
    {
        m_equations.emplace_back(conv->convertToEx(equations[i]));
        m_R.emplace_back(conv->convertToEx(R[i]));
    }
}

System::System(uint dim, const GiNaC::exvector& equations, const GiNaC::exvector& R) : m_dim(dim)
{
    assert(dim == equations.size());
    assert(dim == R.size());
    for (size_t i = 0; i < dim; i++)
    {
        m_equations.push_back(equations[i]);
        m_R.push_back(R[i]);
    }
}

uint System::getDim() const
{
    return m_dim;
}

GiNaC::exvector System::getEquations() const
{
    return m_equations;
}

void System::addEquation(const std::string& eq)
{
    std::unique_ptr<IConverter> conv(new Converter);
    m_equations.emplace_back(conv->convertToEx(eq));
    ++m_dim;
}

void System::addEquation(const GiNaC::ex& eq)
{
    m_equations.push_back(eq);
    ++m_dim;
}

void System::removeEquation(size_t eqIndex)
{
    assert(eqIndex <= m_equations.size());
    m_equations.erase(m_equations.begin() + eqIndex);
    --m_dim;
}

void System::addR(const std::string& r)
{
    std::unique_ptr<IConverter> conv(new Converter);
    m_R.emplace_back(conv->convertToEx(r));
}

void System::addR(const GiNaC::ex& r)
{
    m_R.emplace_back(r);
}

void System::removeR(size_t rIndex)
{
    assert(rIndex <= m_R.size());
    m_R.erase(m_R.begin() + rIndex);
}

GiNaC::exvector System::getR() const
{
    return m_R;
}

void System::setTa(double ta)
{
    m_ta = ta;
}

void System::setTb(double tb)
{
    m_tb = tb;
}

void System::setTs(double ts)
{
    m_ts = ts;
}

double System::getTa() const
{
    return m_ta;
}

double System::getTb() const
{
    return m_tb;
}

double System::getTs() const
{
    return m_ts;
}

void System::addP0(double p0)
{
    m_p0.push_back(p0);
}
void System::removeP0(size_t p0Index)
{
    assert(p0Index <= m_p0.size());
    m_p0.erase(m_p0.begin() + p0Index);
}

void System::updateP0(size_t p0Index, double p0)
{
    assert(p0Index<= m_p0.size());
    m_p0[p0Index] = p0;
}

std::vector<double> System::getP0() const
{
    return m_p0;
}

} // namespace mpp

