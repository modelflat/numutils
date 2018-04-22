#include "TestUtils.hpp"

#include "NumericalUtils.hpp"

TEST(NumUtilsTest, FunctionSum) {
    double x = 4.2;

    // empty sum is 0
    EXPECT_DOUBLE_EQ(nya::sum()(x), 0.0);

    // handles 1 function
    auto f1 = [](double x) { return x*std::log2(x); };
    EXPECT_DOUBLE_EQ(nya::sum(f1)(x), f1(x));

    // handles 2 functions
    auto f2 = [](double x) { return x; };
    EXPECT_DOUBLE_EQ( nya::sum(f1, f2)(x), f1(x) + f2(x) );

    // handles some arbitrary number of functions with repetitions
    auto f3 = [](double x) { return x*x*x + x; };
    auto f4 = [](double x) { return std::sin(x); };
    EXPECT_DOUBLE_EQ(nya::sum(f1, f2, f4, f1, f3, f2)(x), f1(x) + f2(x) + f4(x) + f1(x) + f3(x) + f2(x));
}

TEST(NumUtilsTest, FunctionProduct) {
    double x = 3.4;

    // empty product is 1.0
    EXPECT_DOUBLE_EQ(nya::product()(x), 1.0);

    // handles 1 function
    auto f1 = [](double x) { return x*std::log2(x); };
    EXPECT_DOUBLE_EQ(nya::product(f1)(x), f1(x));

    // handles 2 functions
    auto f2 = [](double x) { return x; };
    EXPECT_DOUBLE_EQ( nya::product(f1, f2)(x), f1(x)*f2(x) );

    // handles some arbitrary number of functions with repetitions
    auto f3 = [](double x) { return x*x*x + x; };
    auto f4 = [](double x) { return std::sin(x); };
    EXPECT_DOUBLE_EQ(nya::product(f1, f2, f4, f1, f3, f2)(x), f1(x)*f2(x)*f4(x)*f1(x)*f3(x)*f2(x));
}

TEST(NumUtilsTest, FunctionNegation) {
    double x = 2.3;
    auto f = [](auto x) { return x + std::exp(x - 1); };

    EXPECT_DOUBLE_EQ(nya::negate(f)(x), -f(x));
}

TEST(NumUtilsTest, FunctionDerivative) {
    double x = 4;
    auto f = [](auto x) { return x*x - x; };

    // first
    EXPECT_NEAR(nya::D<nya::LFD1>(f)(x), (2.0*x - 1.0), nya::PrecisionTraits<double>::derivativeError());
    // second // todo too big error, fix
//    EXPECT_NEAR(nya::D<nya::>(f)(x), 2.0, PrecisionTraits<double>::derivativeError() * 100);
}

TEST(NumUtilsTest, FunctionIntegral_Euler) {
    auto xRange = nya::discreteRange<6>(0.0, 1.0);
    auto f = [](auto x) { return x*x - x; };

    EXPECT_NEAR(nya::integral<nya::Euler>(f)(xRange), 1.0/3.0 - 1.0/2.0, 1e-8); // todo move absError into Stepper classes
}

TEST(NumUtilsTest, FunctionIntegral_RK4) {
    auto xRange = nya::discreteRange<6>(0.0, 1.0);
    auto f = [](auto x) { return x * x - x; };

    EXPECT_NEAR(nya::integral<nya::RK4>(f)(xRange), 1.0 / 3.0 - 1.0 / 2.0, 1e-10); // todo move absError into Stepper classes
}

TEST(NumUtilsTest, FunctionIntegral_Multi) {
    auto xRange = nya::discreteRange<6>(0.0, 1.0);
    auto f = [](auto x, auto y) { return y*std::sin(x) + x*std::cos(y); };
    // int( f )|x[0,1] : -y*cos(1) - y*cos(0) + cos(y)/2
    // int( f )|y[0,1] : sin(x)/2 + x*sin(1)

    // integral by x; y = 1
    auto f2ByX = nya::integral<nya::RK4, 0>(f);
    EXPECT_NEAR(f2ByX(xRange, 0.0, 1), -std::cos(1) + std::cos(0) + std::cos(1)/2, 1e-6); // todo low precision
    // integral by y; x = 1
    auto f2ByY = nya::integral<nya::RK4, 1>(f);
    EXPECT_NEAR(f2ByY(xRange, 1, 0.0), std::sin(1)/2 + std::sin(1), 1e-6); // todo low precision
}

TEST(NumUtilsTest, FunctionInnerProduct) {
    auto xRange = nya::discreteRange<6>(0.0, 1.0);
    auto f = [](auto x) { return x; };
    auto g = [](auto x) { return std::exp(x); };

    // integral( x*exp(x) ) = x*exp(x) - int(exp(x)) = x*exp(x)|0,1 - exp(x)|0,1 = exp(1) - exp(1) - exp(0) = 1.0
    EXPECT_NEAR(nya::innerProduct<nya::RK4>(f, g)(xRange), 1.0, 1e-6); // todo move absError into Stepper classes
}

TEST(NumUtilsTest, FunctionPower) {
    auto fxSquared = nya::power([](auto x) { return x; }, 2);
    EXPECT_DOUBLE_EQ(fxSquared(5), 25);

    auto fxNonInt = nya::power([](auto x) { return x; }, 3.123);
    EXPECT_DOUBLE_EQ(fxNonInt(5), std::pow(5, 3.123));

    auto fxNegative = nya::power([](auto x) { return x; }, -2);
    EXPECT_DOUBLE_EQ(fxNegative(5), 1.0 / 25);

    auto fxZero = nya::power([](auto x) { return x + x*x; }, 0);
    EXPECT_DOUBLE_EQ(fxZero(5), 1);
}