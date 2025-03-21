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
// выражение в expression
    static std::shared_ptr<Expression> parse(const std::string& expr);
private:
// отвечает за приоретет операторов
    static int getPrecedence(char op);
    //для операторов
    static std::shared_ptr<Expression> applyOperator(char op, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right);
    //разделение на токены
    static std::vector<std::string> tokenize(const std::string& expr);
};

//вычисляет значение выражения
std::string evaluateExpression(const std::string& expr, const std::map<std::string, double>& variables);

// вычисляет символьную производную
std::string differentiateExpression(const std::string& expr, const std::string& var);

#endif // EXPRESSION_PARSER_HPP
