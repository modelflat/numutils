#include "TestUtils.hpp"

#include <Range.hpp>

// range initializes with proper step size and count for various step orders
TEST(RangeTest, DiscreteRange_GoodInterval) {
    auto r1 = nya::discreteRange<0>(0.0, 1.0);
    EXPECT_EQ(r1.count(), 1);
    EXPECT_DOUBLE_EQ(r1.step(), 1.0);
    auto r2 = nya::discreteRange<1>(0.0, 1.0);
    EXPECT_EQ(r2.count(), 10);
    EXPECT_DOUBLE_EQ(r2.step(), 0.1);
    auto r3 = nya::discreteRange<6>(0.0, 1.0);
    EXPECT_EQ(r3.count(), 1'000'000);
    EXPECT_DOUBLE_EQ(r3.step(), 1e-6);
}

TEST(RangeTest, DiscreteRange_EvilInterval) {
    auto r1 = nya::discreteRange<0>(0.0101010101, 1.0101010101);
    EXPECT_EQ(r1.count(), 1);
    EXPECT_DOUBLE_EQ(r1.step(), 1.0);
    auto r2 = nya::discreteRange<6>(1.7942, 1.7943);
    EXPECT_EQ(r2.count(), 1'000'000);
    EXPECT_NEAR(r2.step(), 1e-10, 1e-20);
}

// [a, b] when a > b
TEST(RangeTest, DiscreteRange_InverseInterval) {
    auto r1 = nya::discreteRange<2>(2.0, 1.0);
    EXPECT_EQ(r1.count(), 100);
    EXPECT_TRUE(r1.step() < 0);
    EXPECT_DOUBLE_EQ(r1.step(), -.01);
}

TEST(RangeTest, IteratorTest_GoodRange) {
    nya::Range r { 0.0, 100, 1.0 };
    EXPECT_EQ(std::distance( r.begin(), r.end() ), 100);
    EXPECT_DOUBLE_EQ(*r.begin(), 0.0);
    EXPECT_DOUBLE_EQ(*r.end(), 100.0);

    // operators
    auto it = r.begin();
    EXPECT_DOUBLE_EQ(*(it + 20), 20.0);
    // advance
    EXPECT_DOUBLE_EQ(*(++it), 1.0);
    EXPECT_DOUBLE_EQ(*(it++), 1.0);
    EXPECT_DOUBLE_EQ(*it, 2.0);
    EXPECT_DOUBLE_EQ(*(--it), 1.0);
    EXPECT_DOUBLE_EQ(*(it--), 1.0);
    EXPECT_DOUBLE_EQ(*it, 0.0);
    // advance N
    it += 17;
    EXPECT_DOUBLE_EQ(*it, 17.0);
    it -= 13;
    EXPECT_DOUBLE_EQ(*it, 4.0);
}

TEST(RangeTest, IteratorTest_EvilRange) {
    nya::Range r { 0.3, 100, 0.010007 };

    EXPECT_DOUBLE_EQ(*r.begin(), 0.3);
    EXPECT_DOUBLE_EQ(*r.end(), 1.3007);

    // operators
    auto it = r.begin();
    EXPECT_DOUBLE_EQ(*(it + 20), 0.50014);
    // advance
    EXPECT_DOUBLE_EQ(*(++it), 0.310007);
    EXPECT_DOUBLE_EQ(*(it++), 0.310007);
    EXPECT_DOUBLE_EQ(*it, 0.320014);
    EXPECT_DOUBLE_EQ(*(--it), 0.310007);
    EXPECT_DOUBLE_EQ(*(it--), 0.310007);
    EXPECT_DOUBLE_EQ(*it, 0.3);
    // advance N
    it += 17;
    EXPECT_DOUBLE_EQ(*it, 0.470119);
    it -= 13;
    EXPECT_DOUBLE_EQ(*it, 0.340028);
}