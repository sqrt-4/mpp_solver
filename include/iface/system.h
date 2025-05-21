#pragma once

#include "iface.h"

#include <ginac/ginac.h>
#include <QString>

namespace mpp {

struct ISystem: Iface
{
    virtual void addEquation(const std::string& eq) = 0;
    virtual void addEquation(const GiNaC::ex& eq) = 0;
    virtual void removeEquation(size_t eqIndex) = 0;
    virtual GiNaC::exvector getEquations() const = 0;

    virtual void addR(const std::string& r) = 0;
    virtual void addR(const GiNaC::ex& r) = 0;
    virtual void removeR(size_t rIndex) = 0;
    virtual GiNaC::exvector getR() const = 0;

    virtual void setTa(double ta) = 0;
    virtual void setTb(double tb) = 0;
    virtual void setTs(double ts) = 0;
    virtual double getTa() const = 0;
    virtual double getTb() const = 0;
    virtual double getTs() const = 0;

    virtual void addP0(double p0) = 0;
    virtual void removeP0(size_t p0Index) = 0;
    virtual void updateP0(size_t p0Index, double p0) = 0;
    virtual std::vector<double> getP0() const = 0;

    virtual unsigned int getDim() const = 0;
};

} // namespace mpp

namespace gui {

using Equations = std::vector<QString>;

struct ISystem: Iface
{
    virtual void addEquation(const QString& eq) = 0;
    virtual void removeEquation(size_t eqIndex) = 0;
    virtual Equations getEquations() const = 0;
    virtual void updateEquation(size_t eqIndex, const QString& eq) = 0;


    virtual void addR(const QString& r) = 0;
    virtual void removeR(size_t rIndex) = 0;
    virtual void updateR(size_t eqIndex, const QString& r) = 0;
    virtual Equations getR() const = 0;

    virtual void addP0(double p0) = 0;
    virtual void removeP0(size_t p0Index) = 0;
    virtual void updateP0(size_t p0Index, double p0) = 0;
    virtual std::vector<double> getP0() const = 0;

    virtual void setTa(double ta) = 0;
    virtual void setTb(double tb) = 0;
    virtual void setTs(double ts) = 0;
    virtual double getTa() const = 0;
    virtual double getTb() const = 0;
    virtual double getTs() const = 0;
};

} // namespace gui
