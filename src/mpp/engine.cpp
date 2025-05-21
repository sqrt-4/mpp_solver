#include <iface/converter.h>
#include <helpers/converter.h>
#include <symbols.h>
#include <mpp/systemModel.h>
#include <mpp/engine.h>
#include <fstream>
#include <utility>
#include <algorithm>

namespace {
static const unsigned int INTERNAL_STEPS = 200;
static const unsigned int MU_STEPS = 100;
static const unsigned int EXTERNAL_ITERS = 3;
}

namespace mpp {

using namespace GiNaC;

double norm(const PointType& x)
{
    double res = 0;
    for (auto it : x)
    {
        res += it*it;
    }
    return sqrt(res);
}

PointType Engine::make_rk4_iter(ISystem* system, double t, mpp::PointType y, double dt)
{
    using namespace GiNaC;
    PointType k1 = systemODE(system, t, y);
    PointType y_temp(y.size());
    for (size_t i = 0; i < y.size(); ++i)
        y_temp[i] = y[i] + 0.5 * dt * k1[i];

    PointType k2 = systemODE(system, t + 0.5 * dt, y_temp);

    for (size_t i = 0; i < y.size(); ++i)
        y_temp[i] = y[i] + 0.5 * dt * k2[i];

    PointType k3 = systemODE(system, t + 0.5 * dt, y_temp);

    for (size_t i = 0; i < y.size(); ++i)
        y_temp[i] = y[i] + dt * k3[i];

    PointType k4 = systemODE(system, t + dt, y_temp);
    for (size_t i = 0; i < y.size(); ++i)
        y[i] += dt * (k1[i] + 2*k2[i] + 2*k3[i] + k4[i]) / 6.0;

    return y;
}

PointType Engine::systemODE(ISystem* system, double t, PointType x)
{
    auto n = system->getDim();
    const auto ftx = system->getEquations();
    const auto J = compute_jacobian(ftx, system->getDim());
    GiNaC::matrix X(n,n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            X(i,j) = x[n + i * n + j];

    PointType x_sys(x.begin(), x.begin() + n);
    std::unique_ptr<mpp::IConverter> conv(new mpp::Converter);
    auto x_map = conv->convertToExmap(x_sys);

    for (int i = 0; i < n; ++i)
    {
        x[i] = ex_to<numeric>(ftx[i].subs(x_map).evalf()).to_double();
    }
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j) {
            double sum = 0.0;
            for (int k = 0; k < n; ++k)
                sum += ex_to<numeric>(J(i,k).subs(x_map).evalf()).to_double() * ex_to<numeric>(X(k,j).subs(x_map).evalf()).to_double();
            x[n + i * n + j] = sum;
        }

    return x;
}

std::vector<PointType> Engine::solve_inside_a(ISystem* system, PointType x)
{
    const double dt = -fabs(system->getTa() - system->getTs()) / INTERNAL_STEPS;
    std::vector<PointType> res;
    res.push_back(x);
    for (int i = 0; i < INTERNAL_STEPS; ++i)
    {
        x = make_rk4_iter(system, 0, x, dt);
        res.emplace_back(x.begin(), x.end());
    }
    return res;
}

std::vector<PointType> Engine::solve_inside_b(ISystem* system, PointType x)
{
    const double dt = fabs(system->getTb() - system->getTs()) / INTERNAL_STEPS;
    std::vector<PointType> res;
    res.push_back(x);
    for (int i = 0 ; i < INTERNAL_STEPS; ++i)
    {
        x = make_rk4_iter(system, 0, x, dt);
        res.emplace_back(x.begin(), x.end());
    }

    return res;
}

GiNaC::matrix compute_jac_new(const GiNaC::exvector& F, unsigned int dim, int m)
{
    const auto num_funcs = F.size();
    GiNaC::matrix jacobian(num_funcs, dim);
    for (int i = 0; i < num_funcs; i++)
    {
        for (int j = 0; j < dim; ++j)
        {
            jacobian(i,j) = F[i].diff(Symbols::SYM[m + j + 1]);
        }
    }
    return jacobian;
}

matrix Engine::build_Rx(const exvector& R)
{
    return compute_jac_new(R, R.size(), 0);
}

matrix Engine::build_Ry(const exvector& R)
{
    return compute_jac_new(R, R.size(), R.size());
}

std::vector<PointType> Engine::solve_inside(ISystem* system, const PointType& initConditionsForInternalTask)
{
    std::vector<PointType> x_int;
    if (fabs(system->getTa() - system->getTs()) <= 0.0001)
    {
        auto xb = solve_inside_b(system, initConditionsForInternalTask);
        x_int = std::move(xb);
    }
    else if (fabs(system->getTb() - system->getTs()) <= 0.0001)
    {
        auto xa = solve_inside_a(system, initConditionsForInternalTask);
        x_int = std::move(xa);
        std::reverse(x_int.begin(), x_int.end());
    }
    else
    {
        auto xa = solve_inside_a(system, initConditionsForInternalTask);
        auto xb = solve_inside_b(system, initConditionsForInternalTask);
        x_int = std::vector<PointType>(xa.begin() + 1, xa.end());
        std::reverse(x_int.begin(), x_int.end());
        std::vector<PointType> x_res;
        x_res.reserve(x_int.size() + xb.size());
        std::merge(x_int.begin(), x_int.end(), xb.begin(), xb.end(), std::back_inserter(x_res));
        x_int = std::move(x_res);
    }
    return x_int;
}

PointType Engine::build_initConditionsForInternalTask(const PointType& pk, size_t dim)
{
    auto initConditionsForInternalTask = pk;
    for (int i = 0; i < dim; ++i)
    {
        for (int j = 0; j < dim; ++j)
        {
            initConditionsForInternalTask.push_back(0);
        }
        initConditionsForInternalTask[dim + i * dim + i] = 1;
    }
    return initConditionsForInternalTask;
}

void Engine::solve_external(ISystem* system, const std::vector<PointType>& x_int, PointType& pk)
{
    double mu = 0.;
    double h = 1. / MU_STEPS;
    const auto n = system->getDim();
    PointType xp0;
    GiNaC::matrix xAp(n, n);
    {
        int idx = 0;
        for (int i = 0; i < n; ++i)
        {
            xp0.push_back(x_int[0][i]);
            for (int j = 0; j < n; ++j)
                xAp(i,j) = x_int[0][n + idx++];
        }
    }

    GiNaC::matrix xBp(n, n);
    {
        int idx = 0;
        size_t last_idx = x_int.size() - 1;
        for (int i = 0; i < n; ++i)
        {
            xp0.push_back(x_int[last_idx][i]);
            for (int j = 0; j < n; ++j)
                xBp(i,j) = x_int[last_idx][n + idx++];
        }
    }

    const auto F = system->getR();
    GiNaC::matrix F0(n, 1);
    std::unique_ptr<mpp::IConverter> conv(new mpp::Converter);
    auto exmap_x = conv->convertToExmap2(xp0);
    for (size_t i = 0; i < F0.rows(); ++i)
    {
        F0[i] = F[i].subs(exmap_x);
    }

    auto R_vec = system->getR();
    matrix R_x = build_Rx(R_vec);
    matrix R_y = build_Ry(R_vec);
    matrix R1 = R_x.mul(xAp);
    matrix R2 = R_y.mul(xBp);
    GiNaC::matrix F_ = R1.add(R2);

    GiNaC::matrix F_inverse = F_.inverse();

    GiNaC::matrix rhs_sys = F_inverse.mul(F0);
    for (size_t i = 0; i < system->getDim(); ++i)
    {
        rhs_sys[i] = -rhs_sys[i];
    }

    for (int i = 0; i <= MU_STEPS; i++, mu += h)
    {
        pk = make_rk23_iter(system, rhs_sys, pk, h);
    }
}

std::vector<PointType> Engine::compute_solution(ISystem* system)
{
    std::vector<std::ofstream> files;
    std::ofstream f("mu.txt", std::ios::out);
    files.emplace_back(std::move(f));
    for (size_t i = 0; i < system->getDim(); i++)
    {
        std::string fname = "x" + std::to_string(i+1) + ".txt";
        std::ofstream f(fname, std::ios::out);
        files.emplace_back(std::move(f));
    }

    auto pk = system->getP0();
    
    std::vector<PointType> x_int;
    for (int z = 0; z < EXTERNAL_ITERS; ++z) 
    {
        auto initConditionsForInternalTask = build_initConditionsForInternalTask(pk, system->getDim());
        x_int = solve_inside(system, initConditionsForInternalTask);
        solve_external(system, x_int, pk);
    }

    std::vector<PointType> res(x_int.size());
    double t = system->getTa();
    const double dt = fabs(system->getTa() - system->getTb()) / INTERNAL_STEPS;
    for (size_t i = 0; i < x_int.size(); ++i)
    {
        res[i].resize(system->getDim() + 1);
        files[0] << t << "\n";
        res[i][0] = t;
        for (size_t j = 0; j < system->getDim(); ++j)
        {
            res[i][j+1] = x_int[i][j];
        files[j+1] << res[i][j+1] << "\n";
        }
        t += dt;
    }

    return res;
}


GiNaC::matrix Engine::compute_jacobian(const GiNaC::exvector& F, unsigned int dim)
{
    const auto num_funcs = F.size();
    GiNaC::matrix jacobian(num_funcs, dim);
    for (int i = 0; i < num_funcs; i++)
    {
        for (int j = 0; j < dim; ++j)
        {
            jacobian(i, j) = F[i].diff(Symbols::SYM[j+1]);
        }
    }
    return jacobian;
}

PointType Engine::make_rk23_iter(ISystem* system, const GiNaC::matrix& rhs_sys, const PointType& fromPoint, double h_d)
{
    using namespace GiNaC;
    const auto dim = system->getDim();
    std::unique_ptr<IConverter> conv(new Converter);
    auto pk = conv->convertToExmap(fromPoint);
    double h = h_d;
    double acc = -1;
    PointType res;
    do {
    if (acc > 0)
    {
        h *= ex_to<numeric>(pow(h / acc, 1./3)).to_double();
    }
    PointType k1, yn, k11, f_val;
    f_val.reserve(dim);
    yn.reserve(dim);
    k1.reserve(dim);
    k11.reserve(dim);


    for (size_t i = 0; i < dim; ++i)
    {
        f_val.push_back(ex_to<numeric>(rhs_sys.subs(pk).evalf()[i]).to_double());
        k1.push_back(h*f_val[i]);
        k11.push_back(fromPoint[i] + 0.5*k1[i]);
    }

    auto pk1 = conv->convertToExmap(k11);
    PointType k2, k22, k3;
    k2.reserve(dim);
    k3.reserve(dim);
    k22.reserve(dim);
    for (size_t i = 0; i < dim; ++i)
    {
        k2.push_back(h*ex_to<numeric>(rhs_sys.subs(pk1).evalf()[i]).to_double());
        k22.push_back(fromPoint[i] - k1[i] + 2*k2[i]);
    }
    auto pk2 = conv->convertToExmap(k22);
    for (size_t i = 0; i < dim; ++i)
    {
        k3.push_back(h*ex_to<numeric>(rhs_sys.subs(pk2).evalf()[i]).to_double());
    }

    PointType pkVal2;
    PointType pkVal3;

    for (size_t i = 0; i < dim; ++i)
    {
        pkVal2.push_back(fromPoint[i] + (k1[i]+k2[i]) / 2.0);
        pkVal3.push_back(fromPoint[i] + (1./6)*k1[i] + (2./3)*k2[i] + (1./6)*k3[i]);
    }
    acc = fabs(norm(pkVal2) - norm(pkVal3));
    res = std::move(pkVal3);
    } while (acc >= 0.001);

    return res;
}

} // namespace mpp
