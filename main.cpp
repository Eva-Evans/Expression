#include "ExpressionParser.hpp"
#include <iostream>
#include <cmath>
#include <map>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Использование:\n"
                  << "  differentiator --eval \"выражение\" var1=val1 var2=val2 ...\n"
                  << "  differentiator --diff \"выражение\" --by var\n";
        return 1;
    }
    
    std::string mode = argv[1];
    if (mode == "--eval" && argc >= 3) { // Если выбран режим вычисления значения выражения
        std::string expression = argv[2];
        std::map<std::string, double> variables;
        for (int i = 3; i < argc; ++i) { // Перебираем аргументы
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
        if (variables.empty()) { // Если переменные не заданы
            std::cerr << "Ошибка: Не заданы значения переменных.\n";
            return 1;
        }
    
        std::cout << evaluateExpression(expression, variables) << std::endl;
    }
    else if (mode == "--diff" && argc >= 5 && std::string(argv[3]) == "--by") { // Если выбран режим вычисления производной
        std::string expression = argv[2];
        std::string variable = argv[4];
        std::cout << differentiateExpression(expression, variable) << std::endl;
    } 
    else { // Если аргументы неверные
        std::cerr << "Неверные аргументы. Используйте --eval или --diff.\n";
        return 1;
    }
    return 0;
}
