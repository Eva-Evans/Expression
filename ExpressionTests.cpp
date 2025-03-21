// ExpressionTests.cpp
#include "Expression.hpp"
#include "ExpressionParser.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

void testConstants() {
    auto expr = ExpressionParser::parse("5");
    assert(expr->evaluate(0) == 5);
    assert(expr->evaluate(10) == 5);
    assert(expr->derivative()->evaluate(0) == 0);
    std::cout << "testConstants passed!\n";
}

void testVariable() {
    auto expr = ExpressionParser::parse("x");
    assert(expr->evaluate(2) == 2);
    assert(expr->evaluate(5) == 5);
    assert(expr->derivative()->evaluate(0) == 1);
    std::cout << "testVariable passed!\n";
}

void testAddition() {
    auto expr = ExpressionParser::parse("3 + x");
    assert(expr->evaluate(2) == 5);
    assert(expr->evaluate(4) == 7);
    assert(expr->derivative()->evaluate(0) == 1);
    std::cout << "testAddition passed!\n";
}

void testMultiplication() {
    auto expr = ExpressionParser::parse("3 * x");
    assert(expr->evaluate(2) == 6);
    assert(expr->evaluate(4) == 12);
    assert(expr->derivative()->evaluate(2) == 3);
    std::cout << "testMultiplication passed!\n";
}

void testDivision() {
    auto expr = ExpressionParser::parse("x / 2");
    assert(expr->evaluate(4) == 2);
    assert(expr->evaluate(10) == 5);
    assert(abs(expr->derivative()->evaluate(2) - 0.5) < 1e-6);
    std::cout << "testDivision passed!\n";
}

void testTrigFunctions() {
    auto exprSin = ExpressionParser::parse("sin(x)");
    double sin_val = exprSin->evaluate(M_PI / 2);
    std::cout << "sin(" << M_PI / 2 << ") = " << sin_val << std::endl;
    assert(abs(sin_val - 1) < 1e-6);

    auto exprCos = ExpressionParser::parse("cos(x)");
    double cos_val = exprCos->evaluate(M_PI);
    std::cout << "cos(" << M_PI << ") = " << cos_val << std::endl;
    assert(abs(cos_val + 1) < 1e-6);
    std::cout << "testTrigFunctions passed!\\n";
}

void testExponential() {
    auto expr = ExpressionParser::parse("exp(x)");
    assert(abs(expr->evaluate(0) - 1) < 1e-6);
    assert(abs(expr->evaluate(1) - M_E) < 1e-6);
    assert(abs(expr->derivative()->evaluate(0) - 1) < 1e-6);
    std::cout << "testExponential passed!\n";
}

void testPower() {
    auto expr = ExpressionParser::parse("x ^ 2");
    assert(abs(expr->evaluate(3) - 9) < 1e-6);
    assert(abs(expr->evaluate(4) - 16) < 1e-6);
    assert(abs(expr->derivative()->evaluate(3) - 6) < 1e-6);
    std::cout << "testPower passed!\n";
}

void testLogarithm() {
    auto expr = ExpressionParser::parse("log(x)");
    assert(abs(expr->evaluate(M_E) - 1) < 1e-6);
    assert(abs(expr->derivative()->evaluate(1) - 1) < 1e-6);
    std::cout << "testLogarithm passed!\n";
}

int main() {
    testConstants();
    testVariable();
    testAddition();
    testMultiplication();
    testDivision();
    testTrigFunctions();
    testExponential();
    testPower();
    testLogarithm();
    std::cout << "All tests passed successfully!\n";
    return 0;
}
