#include "../include/expression.hpp"
#include <cassert>
#include <iostream>

void testConstant() {
    Expression<double> expr(5.0);
    assert(expr.evaluate({}) == 5.0);
    std::cout << "testConstant: OK\n";
}

void testVariable() {
    Expression<double> expr("x");
    assert(expr.evaluate({{"x", 10.0}}) == 10.0);
    std::cout << "testVariable: OK\n";
}

void testAddition() {
    Expression<double> expr1(5.0);
    Expression<double> expr2("x");
    auto expr = expr1 + expr2;
    assert(expr.evaluate({{"x", 10.0}}) == 15.0);
    std::cout << "testAddition: OK\n";
}

void testDifferentiation() {
    Expression<double> expr = Expression<double>::fromString("x * x");
    auto derivative = expr.derivative("x");
    assert(derivative.evaluate({{"x", 2.0}}) == 4.0); // Производная x^2 = 2x, при x=2 -> 4
    std::cout << "testDifferentiation: OK\n";
}

void testStringParsing() {
    auto expr = Expression<double>::fromString("10 + x");
    assert(expr.evaluate({{"x", 5.0}}) == 15.0);
    std::cout << "testStringParsing: OK\n";
}

int main() {
    testConstant();
    testVariable();
    testAddition();
    testDifferentiation();
    testStringParsing();
    return 0;
}