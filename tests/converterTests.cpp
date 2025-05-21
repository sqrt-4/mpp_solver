#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <helpers/converter.h>
#include <iface/system.h>
#include <gui/systemModel.h>
#include <mpp/systemModel.h>
#include <symbols.h>

class ConverterTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        conv = std::make_unique<mpp::Converter>();
    }

    std::unique_ptr<mpp::IConverter> conv;
};

TEST_F(ConverterTest, convertToExmapTest)
{
    using namespace GiNaC;
    std::vector<double> point = {2.22, 0, 1};

    auto pointMap = conv->convertToExmap(point);

    EXPECT_EQ(pointMap.size(), 3);
    EXPECT_DOUBLE_EQ(ex_to<numeric>(pointMap[Symbols::SYM[1]]).to_double(), point[0]);
    EXPECT_DOUBLE_EQ(ex_to<numeric>(pointMap[Symbols::SYM[2]]).to_double(), point[1]);
    EXPECT_DOUBLE_EQ(ex_to<numeric>(pointMap[Symbols::SYM[3]]).to_double(), point[2]);
}

TEST_F(ConverterTest, convertToExFromString)
{
    using namespace GiNaC;
    std::string sEq = "x1^2 - x2";
    auto eq = conv->convertToEx(sEq);
    std::stringstream ss;

    ss << eq;
    EXPECT_EQ(ss.str(), std::string("-y+x^2"));

    const auto expectedVal = eq.diff(Symbols::SYM[1]).subs(Symbols::SYM[1] == 3);
    EXPECT_EQ(expectedVal, 6);
}

TEST_F(ConverterTest, convertToExFromQstring)
{
    using namespace GiNaC;
    QString sEq = "x1^2 - x2";
    auto eq = conv->convertToEx(sEq);
    std::stringstream ss;

    ss << eq;
    EXPECT_EQ(ss.str(), std::string("-y+x^2"));

    const auto expectedVal = eq.diff(Symbols::SYM[1]).subs(Symbols::SYM[1] == 3);
    EXPECT_EQ(expectedVal, 6);
}

TEST_F(ConverterTest, convertToString)
{
    using namespace GiNaC;
    lst l = {Symbols::SYM[1]};
    ex eq("x - x^2", l);

    EXPECT_EQ(eq.subs(Symbols::SYM[1] == -1), -2);
    auto str = conv->convertToString(eq);
    EXPECT_EQ(str, "x-x^2");
}

TEST_F(ConverterTest, convertToQStringFromEx)
{
    using namespace GiNaC;
    lst l = {Symbols::SYM[1]};
    ex eq("x - x^2", l);

    EXPECT_EQ(eq.subs(Symbols::SYM[1] == -1), -2);
    auto str = conv->convertToQString(eq);
    EXPECT_EQ(str, "x-x^2");
}

TEST_F(ConverterTest, convertToQstringFromVector)
{
    QString str = conv->convertToQstring({0.4, 0, -5});
    EXPECT_EQ(str, "0.4 0 -5 ");
}

TEST_F(ConverterTest, convertToPointType)
{
    using namespace GiNaC;
    exmap pointMap;
    pointMap[Symbols::SYM[1]] = 0.5;
    pointMap[Symbols::SYM[2]] = 0;
    pointMap[Symbols::SYM[3]] = -2;
    const auto dim = 3;
    auto point = conv->convertToPointType(pointMap, dim);
    EXPECT_THAT(point, ::testing::ElementsAre(0.5, 0, -2));
}

TEST_F(ConverterTest, convertToMppModelSystem)
{
    using namespace GiNaC;
    std::unique_ptr<gui::ISystem> sys(new gui::System());
    sys->addEquation("x1-x2^2");
    sys->addEquation("x2-x1^2");
    std::unique_ptr<mpp::ISystem> res_sys(conv->convertToMppModel(sys.get()));

    EXPECT_EQ(res_sys->getDim(), 2);
    const auto equations = res_sys->getEquations();
    EXPECT_EQ(equations[0].subs(Symbols::SYM[1] == 2).subs(Symbols::SYM[2] == -1), 1);
    EXPECT_EQ(equations[1].subs(Symbols::SYM[1] == 2).subs(Symbols::SYM[2] == -1), -5);
}
