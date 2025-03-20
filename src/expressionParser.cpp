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

std::vector<std::string> ExpressionParser::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::stringstream ss(expr);
    char c;
    while (ss >> c) {
        if (isdigit(c) || c == '.') {
            std::string num(1, c);
            while (ss.peek() != EOF && (isdigit(ss.peek()) || ss.peek() == '.')) {
                num += ss.get();
            }
            tokens.push_back(num);
        } else if (isalpha(c)) {
            std::string var(1, c);
            while (ss.peek() != EOF && isalpha(ss.peek())) {
                var += ss.get();
            }
            tokens.push_back(var);
        } else {
            tokens.push_back(std::string(1, c));
        }
    }
    return tokens;
}

std::shared_ptr<Expression> ExpressionParser::parse(const std::string& expr) {
    std::vector<std::string> tokens = tokenize(expr);
    std::stack<std::shared_ptr<Expression>> values;
    std::stack<std::string> ops;
    
    for (const auto& token : tokens) {
        if (isdigit(token[0])) {
            values.push(std::make_shared<Constant>(std::stod(token)));
        } else if (token == "x") {
            values.push(std::make_shared<Variable>());
        } else if (token == "(" ) {
            ops.push("(");
        } else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
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
        } else if (std::string("+-*/^").find(token) != std::string::npos) {
            while (!ops.empty() && getPrecedence(ops.top()[0]) >= getPrecedence(token[0])) {
                auto right = values.top(); values.pop();
                auto left = values.top(); values.pop();
                values.push(applyOperator(ops.top()[0], left, right));
                ops.pop();
            }
            ops.push(token);
        } else if (token == "sin" || token == "cos" || token == "exp" || token == "log") {
            ops.push(token);
        }
    }
    while (!ops.empty()) {
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

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Использование:\n"
                  << "  differentiator --eval \"выражение\" var1=val1 var2=val2 ...\n"
                  << "  differentiator --diff \"выражение\" --by var\n";
        return 1;
    }
    
    std::string mode = argv[1];
    if (mode == "--eval" && argc >= 3) {
        std::string expression = argv[2];
        std::map<std::string, double> variables;
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eq_pos = arg.find('=');
            if (eq_pos != std::string::npos) {
                std::string var = arg.substr(0, eq_pos);
                double val = std::stod(arg.substr(eq_pos + 1));
                variables[var] = val;
                std::cout << var << " = " << val << std::endl;
            }
        }
        
        // Проверка, есть ли нужные переменные
        if (variables.empty()) {
            std::cerr << "Ошибка: Не заданы значения переменных.\n";
            return 1;
        }
    
        std::cout << evaluateExpression(expression, variables) << std::endl;
    }
    else if (mode == "--diff" && argc >= 5 && std::string(argv[3]) == "--by") {
        std::string expression = argv[2];
        std::string variable = argv[4];
        std::cout << differentiateExpression(expression, variable) << std::endl;
    } 
    else {
        std::cerr << "Неверные аргументы. Используйте --eval или --diff.\n";
        return 1;
    }
    return 0;
}
