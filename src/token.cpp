// src/token.cpp
#include <vector>
#include <stdexcept>
#include <cmath>

#include "token.hpp"

using std::string;
using std::vector;

static constexpr size_t kMaxInputLen = 128;

static string removing_spaces(const string &s)
{
    string out;
    out.reserve(s.size());
    for (char c : s)
    {
        if (!isspace((unsigned char)c))
            out.push_back(c);
    }
    if (out.size() > kMaxInputLen)
        throw std::runtime_error("Длина выражения превышает 128 символов");

    return out;
}

vector<Token> lexing(const string &input)
{
    string s = removing_spaces(input);

    vector<Token> out;
    out.reserve(s.size());

    for (size_t i = 0; i < s.size();)
    {
        char c = s[i];

        // число (возможно с десятичной точкой)
        if (isdigit((unsigned char)c) || c == '.')
        {
            size_t start = i;
            bool dot = (c == '.');

            if (dot)
            {
                ++i;
                // точка не должна быть последним символом и должна быть за ней цифра
                if (i == s.size() || !isdigit((unsigned char)s[i]))
                    throw CalcError("Неверный формат числа");
            }

            while (i < s.size() && isdigit((unsigned char)s[i]))
                ++i;

            if (i < s.size() && s[i] == '.')
            {
                if (dot)
                    throw CalcError("Двойная точка в числе");
                dot = true;
                ++i;
                if (i == s.size() || !isdigit((unsigned char)s[i]))
                    throw CalcError("Неверный формат числа");
                while (i < s.size() && isdigit((unsigned char)s[i]))
                    ++i;
            }

            double val;
            val = stod(s.substr(start, i - start));
            // Апостроф после числа -> градусы в радианы
            if (i < s.size() && s[i] == '\'')
            {
                val = val * (acos(-1.0) / 180.0);
                ++i;
            }

            out.push_back(Token::number(val));
            continue;
        }

        if (isalpha((unsigned char)c))
        {
            if (!isLowerAlpha(c))
            {
                throw CalcError("Разрешены только строчные латинские буквы в именах функций и констант");
            }

            // идентификатор (имя функции или константы)
            size_t j = i + 1;
            while (j < s.size() && (isLowerAlpha(s[j]) || isdigit((unsigned char)s[j])))
                ++j;
            string id = s.substr(i, j - i);
            out.push_back(Token::ident(id));
            i = j;
            continue;
        }

        auto make_single = [&](auto token)
        {
            out.push_back(std::move(token));
            ++i;
        };

        switch (c)
        {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            make_single(Token::op(string(1, c)));
            break;
        case '!':
            make_single(Token::fact());
            break;
        case '(':
            make_single(Token::lparen());
            break;
        case ')':
            make_single(Token::rparen());
            break;
        case ',':
            make_single(Token::comma());
            break;
        case '|':
            make_single(Token::bar());
            break;
        default:
        {
            throw CalcError(string("Недопустимый символ: ") + c);
        }
        }
    }
    return out;
}
