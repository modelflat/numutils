#include "NumericalUtils.hpp"
#include <iostream>

int main() {
    // Let's numerically integrate two-argument function by second argument
    // 1. Create callable object representing numerical integral, taken using Runge-Kutta 4th,
    // by second argument of given function. No computations are yet performed.
    auto expXByY = nya::integral<nya::RK4, 1>( [](auto x, auto y) { return std::exp(x)*y; } );
    // 2. Call created object with desired parameters: range of sub-integral var and values for others variables
    // we are required to provide these because we are dealing with numerical integration, not symbolic.
    // For value of y we can use any value (it is simply ignored), but for clearness 'dVar' filler is provided.
    // Analytic result when y=[0.0, 1.0] x=1: exp(1) * 0.5 == 1.3591409142295225
    std::cout << expXByY( nya::discreteRange( 0.0, 1.0 ), 1.0, nya::dVar );
}