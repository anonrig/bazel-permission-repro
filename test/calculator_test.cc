#include "src/calculator.h"

#include <gtest/gtest.h>

namespace calculator {
namespace {

TEST(CalculatorTest, Add) {
  Calculator calc;
  EXPECT_EQ(calc.Add(2, 3), 5);
  EXPECT_EQ(calc.Add(-1, 1), 0);
  EXPECT_EQ(calc.Add(0, 0), 0);
}

TEST(CalculatorTest, Subtract) {
  Calculator calc;
  EXPECT_EQ(calc.Subtract(5, 3), 2);
  EXPECT_EQ(calc.Subtract(1, 1), 0);
  EXPECT_EQ(calc.Subtract(0, 5), -5);
}

TEST(CalculatorTest, Multiply) {
  Calculator calc;
  EXPECT_EQ(calc.Multiply(2, 3), 6);
  EXPECT_EQ(calc.Multiply(-2, 3), -6);
  EXPECT_EQ(calc.Multiply(0, 100), 0);
}

TEST(CalculatorTest, Divide) {
  Calculator calc;
  EXPECT_EQ(calc.Divide(6, 2), 3);
  EXPECT_EQ(calc.Divide(7, 2), 3);  // Integer division
  EXPECT_EQ(calc.Divide(-6, 2), -3);
}

TEST(CalculatorTest, DivideByZero) {
  Calculator calc;
  EXPECT_THROW(calc.Divide(1, 0), std::invalid_argument);
}

}  // namespace
}  // namespace calculator
