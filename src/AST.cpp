// src/AST.cpp
// Выполняет Иванов Константин и Копать Пётр
#include "AST.hpp"

using std::make_shared;
using std::move;
using std::shared_ptr;
using std::string;
using std::vector;

class Parser
{
public:
    Parser(const vector<Token> &tt) : t(tt) {}

    shared_ptr<Node> parse()
    {
        auto n = parseExpr();
        if (!end())
            throw CalcError("Лишние токены в конце выражения");
        return n;
    }

private:
    const vector<Token> &t;
    size_t i = 0;

    bool end() const { return i >= t.size(); }
    const Token &peek() const
    {
        if (end())
            throw CalcError("Неожиданный конец выражения");
        return t[i];
    }
    bool eat(TokType tp)
    {
        if (!end() && t[i].type == tp)
        {
            ++i;
            return true;
        }
        return false;
    }
    bool eatOp(const string &s)
    {
        if (!end() && t[i].type == TokType::OP && t[i].text == s)
        {
            ++i;
            return true;
        }
        return false;
    }

    // expr := add
    shared_ptr<Node> parseExpr() { return parseAdd(); }

    // add := mul (('+'|'-') mul)*
    shared_ptr<Node> parseAdd()
    {
        auto n = parseMul();
        while (!end())
        {
            if (eatOp("+"))
            {
                auto r = parseMul();
                n = Node::binary("+", n, r);
            }
            else if (eatOp("-"))
            {
                auto r = parseMul();
                n = Node::binary("-", n, r);
            }
            else
                break;
        }
        return n;
    }

    // mul := pow (('*'|'/') pow)*
    shared_ptr<Node> parseMul()
    {
        auto n = parsePow();
        while (!end())
        {
            if (eatOp("*"))
            {
                auto r = parsePow();
                n = Node::binary("*", n, r);
            }
            else if (eatOp("/"))
            {
                auto r = parsePow();
                n = Node::binary("/", n, r);
            }
            else
                break;
        }
        return n;
    }

    // pow := unary ('^' pow)?   // правая ассоциативность
    shared_ptr<Node> parsePow()
    {
        auto left = parseUnary();
        if (eatOp("^"))
        {
            auto right = parsePow();
            return Node::binary("^", left, right);
        }
        return left;
    }

    // unary := ('+'|'-') unary | postfix
    shared_ptr<Node> parseUnary()
    {
        if (eatOp("+"))
            return Node::unary("u+", parseUnary());
        if (eatOp("-"))
            return Node::unary("u-", parseUnary());
        return parsePostfix();
    }

    // postfix := primary ('!')*
    shared_ptr<Node> parsePostfix()
    {
        auto n = parsePrimary();
        while (!end() && t[i].type == TokType::FACT)
        {
            ++i;
            n = Node::unary("!", n);
        }
        return n;
    }

    // primary := NUMBER | CONST | FUNC '(' args ')' | '(' expr ')' | '|' expr '|'
    shared_ptr<Node> parsePrimary()
    {
        if (end())
            throw CalcError("Ожидалось выражение");

        if (t[i].type == TokType::NUMBER)
        {
            double v = t[i].value;
            ++i;
            return Node::num(v);
        }

        if (t[i].type == TokType::IDENT)
        {
            string id = t[i].text;
            ++i;
            if (isConstName(id))
                return Node::cnst(id);
            // функция: '(' args ')'
            if (!isFuncName(id))
                throw CalcError("Неизвестная функция или константа: " + id);
            if (!eat(TokType::LPAREN))
                throw CalcError("Ожидалась '(' после имени функции");
            vector<shared_ptr<Node>> args;
            if (!eat(TokType::RPAREN))
            {
                while (true)
                {
                    args.push_back(parseExpr());
                    if (eat(TokType::RPAREN))
                        break;
                    if (!eat(TokType::COMMA))
                        throw CalcError("Ожидалась ',' или ')' в списке аргументов функции");
                }
            }
            // проверка арности
            if (id == "pow" || id == "root" || id == "log")
            {
                if (args.size() != 2)
                    throw CalcError("Функция " + id + " требует ровно 2 аргумента");
            }
            else if (id == "sin" || id == "cos" || id == "tan" || id == "asin" || id == "acos" || id == "atan" || id == "sqrt" || id == "ln" || id == "lg" || id == "abs")
            {
                if (args.size() != 1)
                    throw CalcError("Функция " + id + " требует ровно 1 аргумент");
            }
            return Node::call(id, move(args));
        }

        if (eat(TokType::LPAREN))
        {
            auto n = parseExpr();
            if (!eat(TokType::RPAREN))
                throw CalcError("Скобки не сбалансированы: ожидается ')'");
            return n;
        }

        if (eat(TokType::BAR))
        {
            auto inner = parseExpr();
            if (!eat(TokType::BAR))
                throw CalcError("Отсутствует закрывающий символ '|'");
            vector<shared_ptr<Node>> a;
            a.push_back(inner);
            return Node::call("abs", move(a));
        }

        throw CalcError("Ожидалось число, константа, функция или '('");
    }
};
