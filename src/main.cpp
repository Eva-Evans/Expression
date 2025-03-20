#include "../include/expression.hpp"
#include <iostream>
#include <map>
#include <sstream>

void printUsage() {
    std::cout << "Usage:\n"
              << "  differentiator --eval \"expression\" var1=value1 var2=value2 ...\n"
              << "  differentiator --diff \"expression\" --by variable\n";
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "--eval") {
        if (argc < 3) {
            printUsage();
            return 1;
        }

        std::string exprStr = argv[2];
        auto expr = Expression<double>::fromString(exprStr);

        std::map<std::string, double> variables;
        for (int i = 3; i < argc; ++i) {
            std::string arg = argv[i];
            size_t eqPos = arg.find('=');
            if (eqPos == std::string::npos) {
                std::cerr << "Invalid variable assignment: " << arg << "\n";
                return 1;
            }
            std::string var = arg.substr(0, eqPos);
            double value = std::stod(arg.substr(eqPos + 1));
            variables[var] = value;
        }

        std::cout << expr.evaluate(variables) << "\n";
    } else if (mode == "--diff") {
        if (argc < 5 || std::string(argv[3]) != "--by") {
            printUsage();
            return 1;
        }

        std::string exprStr = argv[2];
        std::string variable = argv[4];
        auto expr = Expression<double>::fromString(exprStr);
        auto derivative = expr.derivative(variable);

        std::cout << derivative.toString() << "\n";
    } else {
        printUsage();
        return 1;
    }

    return 0;
}