#pragma once

#include <QString.h>
#include <vector>
#include <iface/system.h>

namespace gui {

class System: public ISystem
{
private:
    void addEquation(const QString& eq) override;
    void removeEquation(size_t eqIndex) override;
    void updateEquation(size_t eqIndex, const QString& eq) override;
    Equations getEquations() const override;

    void addR(const QString& r) override;
    void removeR(size_t rIndex) override;
    void updateR(size_t eqIndex, const QString& r) override;
    Equations getR() const override;

    double getTa() const override;
    double getTb() const override;
    double getTs() const override;

    void setTa(double ta) override;
    void setTb(double tb) override;
    void setTs(double ts) override;

    void addP0(double p0) override;
    void removeP0(size_t p0Index) override;
    void updateP0(size_t p0Index, double p0) override;
    std::vector<double> getP0() const override;

private:
    Equations m_equations;
    Equations m_R;
    double m_ta;
    double m_tb;
    double m_ts;
    std::vector<double> m_p0;
};

} // namespace gui
