#include "../include/expression.hpp"
#include <cmath>
#include <stdexcept>
#include <stack>
#include <sstream>
#include <cctype>

template<typename T>
Expression<T> Expression<T>::fromString(const std::string& expr) {
    // Простейший парсер (можно расширить для поддержки всех операций)
    std::istringstream iss(expr);
    std::string token;
    iss >> token;

    if (std::isdigit(token[0]) || (token[0] == '-' && std::isdigit(token[1]))) {
        // Если токен — число
        T value;
        std::istringstream(token) >> value;
        return Expression(value);
    } else {
        // Если токен — переменная
        return Expression(token);
    }
}

template<typename T>
struct Expression<T>::Impl {
    virtual ~Impl() = default;
    virtual T evaluate(const std::map<std::string, T>& variables) const = 0;
    virtual std::string toString() const = 0;
    virtual std::shared_ptr<Impl> derivative(const std::string& variable) const = 0;
    virtual std::shared_ptr<Impl> clone() const = 0;
};

template<typename T>
class Constant : public Expression<T>::Impl {
    T value;
public:
    Constant(T value) : value(value) {}
    T evaluate(const std::map<std::string, T>&) const override { return value; }
    std::string toString() const override { return std::to_string(value); }
    std::shared_ptr<typename Expression<T>::Impl> derivative(const std::string&) const override {
        return std::make_shared<Constant>(0);
    }
    std::shared_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_shared<Constant>(*this);
    }
};

template<typename T>
class Variable : public Expression<T>::Impl {
    std::string name;
public:
    Variable(const std::string& name) : name(name) {}
    T evaluate(const std::map<std::string, T>& variables) const override {
        auto it = variables.find(name);
        if (it != variables.end()) return it->second;
        throw std::runtime_error("Variable not found: " + name);
    }
    std::string toString() const override { return name; }
    std::shared_ptr<typename Expression<T>::Impl> derivative(const std::string& variable) const override {
        return std::make_shared<Constant>(variable == name ? 1 : 0);
    }
    std::shared_ptr<typename Expression<T>::Impl> clone() const override {
        return std::make_shared<Variable>(*this);
    }
};


template<typename T>
Expression<T> Expression<T>::derivative(const std::string& variable) const {
    return Expression(pImpl->derivative(variable));
}
// Реализация остальных классов для операций и функций...

template<typename T>
Expression<T>::Expression(T value) : pImpl(std::make_shared<Constant<T>>(value)) {}

template<typename T>
Expression<T>::Expression(const std::string& variable) : pImpl(std::make_shared<Variable<T>>(variable)) {}

// Реализация остальных методов класса Expression...

// Пример реализации оператора сложения
template<typename T>
Expression<T> Expression<T>::operator+(const Expression& other) const {
    // Реализация сложения...
}

// Пример реализации функции sin
template<typename T>
Expression<T> Expression<T>::sin() const {
    // Реализация sin...
}

// Пример реализации метода evaluate
template<typename T>
T Expression<T>::evaluate(const std::map<std::string, T>& variables) const {
    return pImpl->evaluate(variables);
}

// Пример реализации метода toString
template<typename T>
std::string Expression<T>::toString() const {
    return pImpl->toString();
}

// Пример реализации метода substitute
template<typename T>
Expression<T> Expression<T>::substitute(const std::string& variable, T value) const {
    // Реализация подстановки...
}


// Явные инстанциации шаблонов
template class Expression<double>;
template class Expression<std::complex<double>>;