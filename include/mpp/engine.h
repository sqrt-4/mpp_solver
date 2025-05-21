#pragma once

#include <iface/engine.h>
#include <ginac/ginac.h>

namespace mpp {

class Engine: public IEngine
{
protected:
    std::vector<PointType> compute_solution(ISystem* system) override;

    GiNaC::matrix compute_jacobian(const GiNaC::exvector& F, unsigned int dim);
    GiNaC::matrix build_Rx(const GiNaC::exvector& R);
    GiNaC::matrix build_Ry(const GiNaC::exvector& R);
    PointType build_initConditionsForInternalTask(const PointType& pk, size_t dim);

    std::vector<PointType> solve_inside_a(ISystem* system, PointType x);
    std::vector<PointType> solve_inside_b(ISystem* system, PointType x);

    std::vector<PointType> solve_inside(ISystem* system, const PointType& initConditionsForInternalTask);
    void solve_external(ISystem* system, const std::vector<PointType>& x_int, PointType& pk);

    PointType systemODE(ISystem* system, double t, PointType x);
    PointType make_rk4_iter(ISystem* system, double t, mpp::PointType y, double dt);
    PointType make_rk23_iter(ISystem* system, const GiNaC::matrix& rhs_sys, const PointType& fromPoint, double h);
};

} // namespace mpp
