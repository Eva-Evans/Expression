// ExpressionParser.hpp
#ifndef EXPRESSION_PARSER_HPP
#define EXPRESSION_PARSER_HPP

#include "Expression.hpp"
#include <memory>
#include <string>
#include <stack>
#include <map>
#include <sstream>
#include <vector>
#include <cctype>

class ExpressionParser {
public:
    static std::shared_ptr<Expression> parse(const std::string& expr);
private:
    static int getPrecedence(char op);
    static std::shared_ptr<Expression> applyOperator(char op, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
    static std::vector<std::string> tokenize(const std::string& expr);
};

std::string evaluateExpression(const std::string& expr, const std::map<std::string, double>& variables);

// Функция для вычисления символической производной
std::string differentiateExpression(const std::string& expr, const std::string& var);

#endif // EXPRESSION_PARSER_HPP
