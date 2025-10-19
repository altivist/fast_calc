// src/calc.hpp
#pragma once

#include <string>
#include <stdexcept>
#include <unordered_set>

struct CalcError : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

static bool isLowerAlpha(char c) { return c >= 'a' && c <= 'z'; }
static bool isFuncName(const std::string &id)
{
    static const std::unordered_set<std::string> f = {
        "sin", "cos", "tan", "asin", "acos", "atan", "sqrt", "pow", "root", "ln", "lg", "log", "abs"};
    return f.count(id);
}
static bool isConstName(const std::string &id)
{
    return id == "pi" || id == "e" || id == "phi";
}