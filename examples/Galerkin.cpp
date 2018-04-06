#include <NumericalUtils.hpp>
#include <MatPlotLib.hpp>

using namespace nya;

int main() {
    // 1. define how differential operator looks like / compile-time only for now
    auto differentialOperator = [](auto f) { return sum( D(f), negate(f) ); };
    // 2. define vector of trial functions / run-time possible
    auto trialFunctions = simplePolynomials(4);
    // 3. define interval of approximating / run-time possible
    auto interval = Range { 0.0, 2.0 };
    // 4. use Galerkin's method to find an approximating function on some interval / run-time possible
    auto y = galerkin<RK4>( differentialOperator, trialFunctions ) ( interval );

    // plot precise
    plot(interval, [](auto x) { return std::exp(x); }, "r-");
    // plot approximate
    plot(interval, y, "g--");
    // show plots
    mpl::show();
}