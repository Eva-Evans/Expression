// Expression.cpp
#include "Expression.hpp"
#include <iostream>

std::ostream& operator<<(std::ostream& os, const std::shared_ptr<Expression>& expr) {
    os << expr->toString();
    return os;
}
/*
int main() {
    std::shared_ptr<Expression> expr1 = std::make_shared<Constant>(5);
    std::shared_ptr<Expression> expr2 = std::make_shared<Variable>();
    std::shared_ptr<Expression> expr3 = std::make_shared<Addition>(expr1, expr2);
    std::shared_ptr<Expression> expr4 = std::make_shared<Multiplication>(expr3, expr2);
    
    std::cout << "Expression: " << expr4 << std::endl;
    std::cout << "Evaluated at x=2: " << expr4->evaluate(2) << std::endl;
    std::cout << "Derivative: " << expr4->derivative() << std::endl;
    return 0;
}
*/
