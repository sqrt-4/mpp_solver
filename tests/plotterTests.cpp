#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <iface/plotter.h>
#include <helpers/plotter.h>

#include <fstream>

TEST(PlotterTest, createPng)
{
    std::vector<double> point = {2.22, 0, 1};;
    //std::vector<double> ax2 = {1, 2, 3, 4, 5};
    std::ofstream f1("test_x1.txt");
    //EXPECT_TRUE(f1.is_open());
    //std::ofstream f2("test_x2.txt");
    //EXPECT_TRUE(f2.is_open());
    //for (auto i = 0; i < ax1.size(); i++)
    {
        //f1 << ax1[i] << '\n';
        //f2 << ax2[i] << '\n';
    }
    //f1.close();
    //f2.close();

    ////std::unique_ptr<IPlotter> pl = std::make_unique<mpp::Plotter>();
    ////auto filename = pl->createPng("test_x1", "test_x2");
    ////std::ifstream fPng(filename);
    ////EXPECT_TRUE(fPng.is_open());
}
