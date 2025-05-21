#include <gui/systemModel.h>

namespace gui {

// Equations == std::vector<QString>

void System::addEquation(const QString& eq)
{
    m_equations.push_back(eq);
}

void System::removeEquation(size_t eqIndex)
{
    assert(eqIndex <= m_equations.size());
    m_equations.erase(m_equations.begin() + eqIndex);
}

void System::updateEquation(size_t eqIndex, const QString& eq)
{
    assert(eqIndex <= m_equations.size());
    m_equations[eqIndex] = eq;
}

Equations System::getEquations() const
{
    return m_equations;
}

void System::addR(const QString& r)
{
    m_R.push_back(r);
}

void System::removeR(size_t rIndex)
{
    assert(rIndex <= m_R.size());
    m_R.erase(m_R.begin() + rIndex);
}

void System::updateR(size_t rIndex, const QString& r)
{
    assert(rIndex<= m_R.size());
    m_R[rIndex] = r;
}

Equations System::getR() const
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

} // namespace gui
