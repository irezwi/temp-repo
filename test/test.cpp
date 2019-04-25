#include "gtest.h"
#include "../ExtendedPrecission.hpp"

TEST(ConvertingTest, PositiveFloatingPoint)
{
    ExtendedPrecission ep(12.5);

    EXPECT_EQ(0, ep.getSign());
    EXPECT_EQ(16386, ep.getExponent());
    EXPECT_EQ(200, ep.getMantissa());
}

TEST(ConvertingTest, NegativeFloatingPoint)
{
    ExtendedPrecission ep(-12.5);

    EXPECT_EQ(1, ep.getSign());
    EXPECT_EQ(16386, ep.getExponent());
    EXPECT_EQ(200, ep.getMantissa());
}

TEST(ConvertingTest, Zero)
{
    ExtendedPrecission ep(0.0);

    EXPECT_EQ(0, ep.getSign());
    EXPECT_EQ(0, ep.getExponent());
    EXPECT_EQ(0, ep.getMantissa());
}

TEST(EqualOperator, BothSame)
{
    ExtendedPrecission ep1(127.8);
    ExtendedPrecission ep2(127.8);

    EXPECT_EQ(true, ep1 == ep2);
}

TEST(EqualOperator, FirstGreater)
{
    ExtendedPrecission ep1(130.3);
    ExtendedPrecission ep2(127.8);

    EXPECT_EQ(false, ep1 == ep2);
}

TEST(EqualOperator, SecondGreater)
{
    ExtendedPrecission ep1(130.3);
    ExtendedPrecission ep2(1765.8);

    EXPECT_EQ(false, ep1 == ep2);
}

TEST(DISABLED_AddingTest, BothPositive)
{
    ExtendedPrecission ep1(12.5);
    ExtendedPrecission ep2(5.75);

    ExtendedPrecission ep(0.0);
    ep = ep1 + ep2;

    EXPECT_EQ(0, ep.getSign());
    EXPECT_EQ(16387, ep.getExponent());
    EXPECT_EQ(146, ep.getMantissa());
}

TEST(DISABLED_AddingTest, BothNegative)
{
    ExtendedPrecission ep1(12.5);
    ExtendedPrecission ep2(5.75);

    ExtendedPrecission ep(0.0);
    ep = ep1 + ep2;

    EXPECT_EQ(1, ep.getSign());
    EXPECT_EQ(16387, ep.getExponent());
    EXPECT_EQ(146, ep.getMantissa());
}

TEST(DISABLED_AddingTest, FirstPositiveSecondNegative)
{
    ExtendedPrecission ep1(12.5);
    ExtendedPrecission ep2(-5.75);

    ExtendedPrecission ep(0.0);
    ep = ep1 + ep2;

    EXPECT_EQ(0, ep.getSign());
    EXPECT_EQ(16385, ep.getExponent());
    EXPECT_EQ(216, ep.getMantissa());
}

TEST(DISABLED_AddingTest, FirstNegativeSecondPositive)
{
    ExtendedPrecission ep1(12.5);
    ExtendedPrecission ep2(-5.75);

    ExtendedPrecission ep(0.0);
    ep = ep2 + ep1;

    EXPECT_EQ(0, ep.getSign());
    EXPECT_EQ(16385, ep.getExponent());
    EXPECT_EQ(216, ep.getMantissa());
}

GTEST_API_ int main(int argc, char **argv)
{
    printf("Running main() from %s\n", __FILE__);
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}