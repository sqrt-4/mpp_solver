#include <helpers/plotter.h>
#include <fstream>
#include <vector>
#include <cassert>
#include <gnuplot/gnuplot.h>

constexpr auto DATASET_FILENAME = "plotDataSet.txt";

namespace {

void prepareDataset(const std::vector<double>& axis1, const std::vector<double>& axis2)
{
    auto size = axis1.size();
    assert(size == axis2.size());
    std::ofstream dataset(DATASET_FILENAME);
    for (size_t i = 0; i < axis1.size(); i++)
    {
        dataset << axis1[i] << ' ' << axis2[i] << std::endl;
    }
    dataset.close();
}

} // namespace


namespace mpp {

std::string Plotter::createPng(const std::string& axisName1, const std::string& axisName2) const
{
    const std::string ext = ".txt";
    std::ifstream f1(axisName1 + ext);
    std::ifstream f2(axisName2 + ext);
    if (!f1.is_open() || !f2.is_open())
    {
        throw std::runtime_error("plotter: can't open data source");
    }

    GnuplotPipe gp;
    gp.sendLine("set terminal png size 1024,720");
    gp.sendLine("set size square");
    const std::string fPictureName("result.png");
    gp.sendLine("set output \"" + fPictureName + "\"");
    double x1, x2;
    std::vector<double> axv1, axv2;
    while (f1 >> x1 && f2 >> x2)
    {
        axv1.push_back(x1);
        axv2.push_back(x2);
    }
    prepareDataset(axv1, axv2);
    std::string filename(DATASET_FILENAME);
    std::string cmd = "plot \"" + filename + "\" using 1:2 with points pointtype 7 pointsize 1.5 linecolor rgb 'blue' title 'Points'";
    std::cout << "cmd: " << cmd << "\n";
    gp.sendLine(cmd);

    return fPictureName;
}

} // namespace mpp
