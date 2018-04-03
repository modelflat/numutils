#include <iostream>

#include <MatPlotLib.hpp>

int main() {
    std::cout << "Plotting data... " << std::endl;
    mpl::plot({1,3,2,4});
    std::cout << "Showing... " << std::endl;
    mpl::show();
}