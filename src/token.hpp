// src/token.hpp
#pragma once

#include "calc.hpp"

enum class TokType
{
    NUMBER, // любое число
    IDENT, // имя функции/константы (строго нижний регистр)
    OP,    // "+ - * / ^"
    FACT,  // "!"
    LPAREN,
    RPAREN,
    COMMA,
    BAR // '|'
};

struct Token
{
    TokType type;
    std::string text;    // для OP/IDENT
    double value{}; // для NUMBER

    static Token number(double v)
    {
        Token t{TokType::NUMBER};
        t.value = v;
        t.text = std::to_string(v);
        return t;
    }
    static Token ident(const std::string &s) { return Token{TokType::IDENT, s}; }
    static Token op(const std::string &s) { return Token{TokType::OP, s}; }
    static Token fact() { return Token{TokType::FACT, "!"}; }
    static Token lparen() { return Token{TokType::LPAREN, "("}; }
    static Token rparen() { return Token{TokType::RPAREN, ")"}; }
    static Token comma() { return Token{TokType::COMMA, ","}; }
    static Token bar() { return Token{TokType::BAR, "|"}; }

private:
    Token(TokType tt, std::string s = "", double v = 0.0) : type(tt), text(std::move(s)), value(v) {}
};

std::vector<Token> lexing(const std::string &input_raw);