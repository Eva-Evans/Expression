// ExpressionParser.cpp
#include "ExpressionParser.hpp"
#include <iostream>
#include <cmath>
#include <map>

int ExpressionParser::getPrecedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

// Применение оператора к двум операндам
std::shared_ptr<Expression> ExpressionParser::applyOperator(char op, std::shared_ptr<Expression> left, std::shared_ptr<Expression> right) {
    switch (op) {
        case '+': return std::make_shared<Addition>(left, right);
        case '-': return std::make_shared<Addition>(left, std::make_shared<Multiplication>(std::make_shared<Constant>(-1), right));
        case '*': return std::make_shared<Multiplication>(left, right);
        case '/': return std::make_shared<Division>(left, right);
        case '^': return std::make_shared<Power>(left, right);
        default: throw std::runtime_error("Unknown operator");
    }
}

// Разбиение строки на токены
std::vector<std::string> ExpressionParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::stringstream ss(expr);
    char c;
    while (ss >> c) { // Пока есть символы в строке
        if (isdigit(c) || c == '.') {
            std::string num(1, c);
            while (ss.peek() != EOF && (isdigit(ss.peek()) || ss.peek() == '.')) { // Пока следующий символ - цифра или точка
                num += ss.get();
            }
            tokens.push_back(num);
        } else if (isalpha(c)) {
            std::string var(1, c);
            while (ss.peek() != EOF && isalpha(ss.peek())) { // Пока следующий символ - буква
                var += ss.get();
            }
            tokens.push_back(var);
        } else {
            tokens.push_back(std::string(1, c));
        }
    }
    return tokens;
}

// Парсинг выражения
std::shared_ptr<Expression> ExpressionParser::parse(const std::string& expr) {
    std::vector<std::string> tokens = tokenize(expr);
    std::stack<std::shared_ptr<Expression>> values;
    std::stack<std::string> ops;
    
    for (const auto& token : tokens) { // Перебираем токены
        if (isdigit(token[0])) {
            values.push(std::make_shared<Constant>(std::stod(token)));
        } else if (token == "x") {
            values.push(std::make_shared<Variable>());
        } else if (token == "(" ) {
            ops.push("(");
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") { // Пока не встретили '('
                auto right = values.top(); values.pop();
                if (ops.top() == "sin") values.push(std::make_shared<Sin>(right));
                else if (ops.top() == "cos") values.push(std::make_shared<Cos>(right));
                else if (ops.top() == "exp") values.push(std::make_shared<Exponential>(right));
                else if (ops.top() == "log") values.push(std::make_shared<Logarithm>(right));
                else {
                    auto left = values.top(); values.pop();
                    values.push(applyOperator(ops.top()[0], left, right));
                }
                ops.pop();
            }
            ops.pop(); // Убираем '('
        } else if (std::string("+-*/^").find(token) != std::string::npos) { // Если токен - оператор
            while (!ops.empty() && getPrecedence(ops.top()[0]) >= getPrecedence(token[0])) {
                auto right = values.top(); values.pop();
                auto left = values.top(); values.pop();
                values.push(applyOperator(ops.top()[0], left, right));
                ops.pop();
            }
            ops.push(token);
        } else if (token == "sin" || token == "cos" || token == "exp" || token == "log") { // Если токен - функция
            ops.push(token);
        }
    }
    while (!ops.empty()) { // Пока есть операторы
        auto right = values.top(); values.pop();
        if (ops.top() == "sin") values.push(std::make_shared<Sin>(right));
        else if (ops.top() == "cos") values.push(std::make_shared<Cos>(right));
        else if (ops.top() == "exp") values.push(std::make_shared<Exponential>(right));
        else if (ops.top() == "log") values.push(std::make_shared<Logarithm>(right));
        else {
            auto left = values.top(); values.pop();
            values.push(applyOperator(ops.top()[0], left, right));
        }
        ops.pop();
    }
    return values.top();
}

// Функция для вычисления значения выражения
std::string evaluateExpression(const std::string& expr, const std::map<std::string, double>& variables) {
    std::shared_ptr<Expression> parsedExpr = ExpressionParser::parse(expr);
    
    // Проверяем все переменные в словаре
    double result = 0;
    std::cout << parsedExpr->toString() << std::endl;
    for (const auto& [var, val] : variables) {
        result = parsedExpr->evaluate(val);
    }

    return std::to_string(result);
}

// Функция для вычисления символической производной
std::string differentiateExpression(const std::string& expr, const std::string& var) {
    std::shared_ptr<Expression> parsedExpr = ExpressionParser::parse(expr);
    std::shared_ptr<Expression> derivative = parsedExpr->derivative();
    return derivative->toString();
}
