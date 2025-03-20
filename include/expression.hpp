#ifndef EXPRESSION_HPP
#define EXPRESSION_HPP

#include <string>
#include <map>
#include <memory>
#include <complex>

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




class Cos;
class Sin;





    

class Logarithm;     


#endif // EXPRESSION_HPP
