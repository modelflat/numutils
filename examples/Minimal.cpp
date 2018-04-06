#include "NumericalUtils.hpp"
#include <iostream>

int main() {
    auto integral = nya::integral<nya::RK4>( [](auto x) { return std::exp(x); } );
    std::cout << integral( nya::discreteRange( 0.0, 1.0 ) );
}