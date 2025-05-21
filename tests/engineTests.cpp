#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <helpers/converter.h>
#include <mpp/engine.h>
#include <mpp/systemModel.h>
#include <symbols.h>

#define EX1
//#define EX2
//#define EX3

    using namespace GiNaC;

class EngineTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        engine = std::make_unique<mpp::Engine>();
        conv = std::make_unique<mpp::Converter>();
#ifdef EX1
        exvector eqs = {
            conv->convertToEx(std::string("x3")),
            conv->convertToEx(std::string("x4")),
            conv->convertToEx(std::string("-x1*(x1^2 + x2^2)^(-3./2)")),
            conv->convertToEx(std::string("-x2*(x1^2 + x2^2)^(-3./2)"))
        };
        exvector R = {
            conv->convertToEx(std::string("x1 - 2")),
            conv->convertToEx(std::string("x2")),
            conv->convertToEx(std::string("x5 - 1.0738644361")),
            conv->convertToEx(std::string("x6 + 1.0995343576"))
        };

        sys = std::make_unique<mpp::System>(mpp::System(4, eqs, R));

        sys->addP0(2);
        sys->addP0(0);
        sys->addP0(-0.5);
        sys->addP0(0.5);

        sys->setTa(0);
        sys->setTb(7);
        sys->setTs(0);
#elif defined(EX2)
        exvector eqs = {
            conv->convertToEx(std::string("x3*x2")),
            conv->convertToEx(std::string("x3*(-x1 + sin(x2))")),
            conv->convertToEx(std::string("0")),
            conv->convertToEx(std::string("0"))
        };
        exvector R = {
            conv->convertToEx(std::string("x1 - x4")),
            conv->convertToEx(std::string("x2")),
            conv->convertToEx(std::string("x5-x8")),
            conv->convertToEx(std::string("x6"))
        };

        sys = std::make_unique<mpp::System>(mpp::System(4, eqs, R));

        //sys->addP0(2);
        //sys->addP0(0);
        //sys->addP0(6.2832);
        //sys->addP0(2);

        sys->addP0(9);
        sys->addP0(0);
        sys->addP0(6.2832);
        sys->addP0(9);

        sys->setTa(0);
        sys->setTb(1);
        sys->setTs(0);
#elif defined(EX3)
        exvector eqs = {
            conv->convertToEx(std::string("x2")),
            conv->convertToEx(std::string("-x1"))
        };
        exvector R = {
            conv->convertToEx(std::string("x1 - 1")),
            conv->convertToEx(std::string("x2 - 1"))
        };

        sys = std::make_unique<mpp::System>(mpp::System(2, eqs, R));

        sys->addP0(2);
        sys->addP0(2);

        sys->setTa(0);
        sys->setTb(1);
        sys->setTs(0);
#endif
        matrix I_(sys->getDim(), sys->getDim());
        for (int i = 0; i < sys->getDim(); i++)
            I_(i,i) = 1;
        I = I_;
    }

    std::unique_ptr<mpp::IEngine> engine;
    std::unique_ptr<mpp::IConverter> conv;
    std::unique_ptr<mpp::ISystem> sys;
    matrix I;
};

TEST_F(EngineTest, sol)
{
    auto sol = engine->compute_solution(sys.get());
    std::cout << "sol: ";
    for (int j = 0; j < sol.size(); ++j)
    {
    for (int i = 0; i < sys->getDim(); ++i)
    {
        std::cout << sol[j][i+1] << ",";
    }
    std::cout << "\n";
    }
    std::cout << std::endl;
}

