#pragma once

#include <iface/system.h>

#include <string>
#include <vector>
#include <ginac/ginac.h>

using uint = unsigned int;

namespace mpp {

class System: public ISystem
{
public:
    explicit System(uint dim = 0): m_dim(dim) {};
    System(uint dim, const std::vector<std::string>& equations, const std::vector<std::string>& R);
    System(uint dim, const GiNaC::exvector& equations, const GiNaC::exvector& R);

private:
    uint getDim() const override;
    GiNaC::exvector getEquations() const override;

    void addEquation(const std::string& eq) override;
    void addEquation(const GiNaC::ex& eq) override;
    void removeEquation(size_t eqIndex) override;

    void addR(const std::string& r) override;
    void addR(const GiNaC::ex& r) override;
    void removeR(size_t rIndex) override;
    GiNaC::exvector getR() const override;

    void setTa(double ta) override;
    void setTb(double tb) override;
    void setTs(double ts) override;

    double getTa() const override;
    double getTb() const override;
    double getTs() const override;

    void addP0(double p0) override;
    void removeP0(size_t p0Index) override;
    void updateP0(size_t p0Index, double p0) override;
    std::vector<double> getP0() const override;

private:
    uint m_dim;
    GiNaC::exvector m_equations;
    double m_ta;
    double m_tb;
    double m_ts;
    GiNaC::exvector m_R;
    std::vector<double> m_p0;
};

} // namespace mpp
