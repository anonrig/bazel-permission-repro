#include "src/calculator.h"

#include <stdexcept>

namespace calculator {

int Calculator::Add(int a, int b) {
  return a + b;
}

int Calculator::Subtract(int a, int b) {
  return a - b;
}

int Calculator::Multiply(int a, int b) {
  return a * b;
}

int Calculator::Divide(int a, int b) {
  if (b == 0) {
    throw std::invalid_argument("Division by zero");
  }
  return a / b;
}

}  // namespace calculator
