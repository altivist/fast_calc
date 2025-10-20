// src/execute.cpp
#include <sstream>
#include <cmath>

#include "AST.hpp"

using std::string;

static constexpr int kMaxOutputLen = 15;

static double get_const(const string &name)
{
    if (name == "pi")
        return acos(-1.0);
    if (name == "e")
        return exp(1.0);
    if (name == "phi")
        return (1.0 + sqrt(5.0)) / 2.0;
    throw CalcError("Неизвестная константа: " + name);
}

static double fact_checked(double x)
{
    if (std::isnan(x) || std::isinf(x))
        throw CalcError("Аргумент факториала некорректен");
    if (x < 0)
        throw CalcError("Факториал определён только для неотрицательных значений");
    double ix = round(x);
    if (fabs(ix - x) > 1e-12)
        throw CalcError("Факториал допустим только для целых значений");
    if (ix > 170.0)
        throw CalcError("Слишком большое значение для факториала");
    return tgamma(ix + 1.0);
}

static string trim_trailing_zeros(const string &s)
{
    auto pos_e = s.find_first_of("eE");
    string mant = (pos_e == string::npos) ? s : s.substr(0, pos_e);
    string expo = (pos_e == string::npos) ? "" : s.substr(pos_e);
    if (mant.find('.') != string::npos)
    {
        while (!mant.empty() && mant.back() == '0')
            mant.pop_back();
        if (!mant.empty() && mant.back() == '.')
            mant.pop_back();
    }
    return mant + expo;
}

static double eval(const std::shared_ptr<Node> &n)
{
    switch (n->type)
    {
    case NodeType::NUMBER:
        return n->number;
    case NodeType::CONST:
        return get_const(n->const_name);
    case NodeType::UNARY:
    {
        double a = eval(n->kids[0]);
        if (n->op == "u+")
            return +a;
        if (n->op == "u-")
            return -a;
        if (n->op == "!")
            return fact_checked(a);
        throw CalcError("Неизвестный унарный оператор: " + n->op);
    }
    case NodeType::BINARY:
    {
        double a = eval(n->kids[0]);
        double b = eval(n->kids[1]);
        if (n->op == "+")
            return a + b;
        if (n->op == "-")
            return a - b;
        if (n->op == "*")
            return a * b;
        if (n->op == "/")
        {
            if (b == 0.0)
                throw CalcError("Деление на ноль");
            return a / b;
        }
        if (n->op == "^")
        {
            if (a == 0.0 && b == 0.0)
                throw CalcError("0^0 не определено");
            return pow(a, b);
        }
        throw CalcError("Неизвестный бинарный оператор: " + n->op);
    }
    case NodeType::CALL:
    {
        const string &f = n->op;
        auto A = [&](int i)
        { return eval(n->kids[i]); };


        if (f == "sin")
            return sin(A(0));
        if (f == "cos")
            return cos(A(0));
        if (f == "tan")
        {
            double x = A(0);
            double c = cos(x);
            if (fabs(c) < 1e-16)
                throw CalcError("Значение tan имеет полюс при данном аргументе");
            return tan(x);
        }
        if (f == "asin")
        {
            double x = A(0);
            if (x < -1.0 || x > 1.0)
                throw CalcError("Аргумент asin вне диапазона [-1,1]");
            return asin(x);
        }
        if (f == "acos")
        {
            double x = A(0);
            if (x < -1.0 || x > 1.0)
                throw CalcError("Аргумент acos вне диапазона [-1,1]");
            return acos(x);
        }
        if (f == "atan")
            return atan(A(0));
        if (f == "sqrt")
        {
            double x = A(0);
            if (x < 0)
                throw CalcError("Корень из отрицательного числа не определён");
            return sqrt(x);
        }
        if (f == "ln")
        {
            double x = A(0);
            if (x <= 0)
                throw CalcError("Натуральный логарифм определён только для положительных значений");
            return log(x);
        }
        if (f == "lg")
        {
            double x = A(0);
            if (x <= 0)
                throw CalcError("Десятичный логарифм определён только для положительных значений");
            return log10(x);
        }
        if (f == "abs")
            return fabs(A(0));
        if (f == "pow")
            return pow(A(0), A(1));
        if (f == "root")
        {
            double x = A(0), n = A(1);
            if (n == 0.0)
                throw CalcError("Степень корня не может быть нулём");
            if (x < 0 && fmod(n, 2.0) == 0.0)
                throw CalcError("Чётный корень из отрицательного числа не определён");
            return pow(x, 1.0 / n);
        }
        if (f == "log")
        {
            double x = A(0), base = A(1);
            if (x <= 0)
                throw CalcError("Логарифм определён только для положительных значений");
            if (base <= 0 || base == 1.0)
                throw CalcError("Основание логарифма должно быть положительным и не равно 1");
            return log(x) / log(base);
        }
        throw CalcError("Неизвестная функция: " + f);
    }
    }
    throw CalcError("Внутренняя ошибка AST");
}


static string format_number(double x)
{
    if (std::isnan(x))
        throw CalcError("Результат не является числом");
    if (std::isinf(x))
        throw CalcError("Результат слишком велик по модулю");

    for (int prec = 15; prec >= 1; --prec)
    {
        std::ostringstream oss;
        oss.setf(std::ios::fmtflags(0), std::ios::floatfield);
        oss.precision(prec);
        oss << x;
        string s = trim_trailing_zeros(oss.str());
        if ((int)s.size() <= kMaxOutputLen)
            return s;
    }
    for (int prec = 15; prec >= 1; --prec)
    {
        std::ostringstream oss;
        oss.setf(std::ios::scientific, std::ios::floatfield);
        oss.precision(prec);
        oss << x;
        string s = trim_trailing_zeros(oss.str());
        if ((int)s.size() <= kMaxOutputLen)
            return s;
    }
    throw CalcError("Невозможно вывести число в 15 символов");
}
