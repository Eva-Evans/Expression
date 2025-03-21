// Expression.hpp
#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <memory>
#include <string>

class Expression {
public:
    virtual ~Expression() = default;
    virtual double evaluate(double x) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<Expression> derivative() const = 0;
};

class Constant : public Expression {
private:
    double value;
public:
    explicit Constant(double v) : value(v) {}
    double evaluate(double x) const override { return value; }
    std::string toString() const override { return std::to_string(value); }
    std::shared_ptr<Expression> derivative() const override { return std::make_shared<Constant>(0); }
};

class Variable : public Expression {
public:
    double evaluate(double x) const override { return x; }
    std::string toString() const override { return "x"; }
    std::shared_ptr<Expression> derivative() const override { return std::make_shared<Constant>(1); }
};

class Addition : public Expression {
private:
    std::shared_ptr<Expression> left, right;
public:
    Addition(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(l), right(r) {}
    double evaluate(double x) const override { return left->evaluate(x) + right->evaluate(x); }
    std::string toString() const override { return "(" + left->toString() + " + " + right->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Addition>(left->derivative(), right->derivative());
    }
};

class Multiplication : public Expression {
private:
    std::shared_ptr<Expression> left, right;
public:
    Multiplication(std::shared_ptr<Expression> l, std::shared_ptr<Expression> r) : left(l), right(r) {}
    double evaluate(double x) const override { return left->evaluate(x) * right->evaluate(x); }
    std::string toString() const override { return "(" + left->toString() + " * " + right->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Addition>(
            std::make_shared<Multiplication>(left->derivative(), right),
            std::make_shared<Multiplication>(left, right->derivative())
        );
    }
};

    
class Division : public Expression {
private:
    std::shared_ptr<Expression> numerator, denominator;
public:
    Division(std::shared_ptr<Expression> num, std::shared_ptr<Expression> denom) : numerator(num), denominator(denom) {}
    double evaluate(double x) const override { return numerator->evaluate(x) / denominator->evaluate(x); }
    std::string toString() const override { return "(" + numerator->toString() + " / " + denominator->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Division>(
            std::make_shared<Addition>(
                std::make_shared<Multiplication>(numerator->derivative(), denominator),
                std::make_shared<Multiplication>(std::make_shared<Constant>(-1),
                                                    std::make_shared<Multiplication>(numerator, denominator->derivative()))
            ),
            std::make_shared<Multiplication>(denominator, denominator)
        );
    }
};

class Cos;

class Sin : public Expression {
private:
    std::shared_ptr<Expression> expr;
public:
    explicit Sin(std::shared_ptr<Expression> e) : expr(e) {}
    double evaluate(double x) const override { return std::sin(expr->evaluate(x)); }
    std::string toString() const override { return "sin(" + expr->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Multiplication>(std::make_shared<Cos>(expr), expr->derivative());
    }
};

class Cos : public Expression {
private:
    std::shared_ptr<Expression> expr;
public:
    explicit Cos(std::shared_ptr<Expression> e) : expr(e) {}
    double evaluate(double x) const override { return std::cos(expr->evaluate(x)); }
    std::string toString() const override { return "cos(" + expr->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Multiplication>(
            std::make_shared<Constant>(-1),
            std::make_shared<Multiplication>(std::make_shared<Sin>(expr), expr->derivative())
        );
    }
};

class Exponential : public Expression {
    private:
        std::shared_ptr<Expression> expr;
    public:
        explicit Exponential(std::shared_ptr<Expression> e) : expr(e) {}
        double evaluate(double x) const override { return std::exp(expr->evaluate(x)); }
        std::string toString() const override { return "exp(" + expr->toString() + ")"; }
        std::shared_ptr<Expression> derivative() const override {
            return std::make_shared<Multiplication>(std::make_shared<Exponential>(expr), expr->derivative());
        }
    };
    

class Logarithm;     
class Power : public Expression {
private:
    std::shared_ptr<Expression> base, exponent;
public:
    Power(std::shared_ptr<Expression> b, std::shared_ptr<Expression> e) : base(b), exponent(e) {}
    double evaluate(double x) const override { return std::pow(base->evaluate(x), exponent->evaluate(x)); }
    std::string toString() const override { return "(" + base->toString() + " ^ " + exponent->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Multiplication>(
            std::make_shared<Power>(base, exponent),
            std::make_shared<Addition>(
                std::make_shared<Multiplication>(exponent->derivative(),
                                                    std::make_shared<Logarithm>(base)),
                std::make_shared<Multiplication>(exponent, 
                                                    std::make_shared<Division>(base->derivative(), base))
            )
        );
    }
};

class Logarithm : public Expression {
private:
    std::shared_ptr<Expression> expr;
public:
    explicit Logarithm(std::shared_ptr<Expression> e) : expr(e) {}
    double evaluate(double x) const override { return std::log(expr->evaluate(x)); }
    std::string toString() const override { return "log(" + expr->toString() + ")"; }
    std::shared_ptr<Expression> derivative() const override {
        return std::make_shared<Division>(expr->derivative(), expr);
    }
};
    

#endif // EXPRESSION_HPP


