// src/AST.hpp
#pragma once

#include <string>
#include <vector>
#include <memory>

#include "token.hpp"

enum class NodeType
{
    NUMBER,
    CONST,
    UNARY,
    BINARY,
    CALL
};

struct Node
{
    NodeType type;
    std::string op;                          // "+","-","*","/","^","!","u+","u-" или имя функции (для CALL)
    double number{};                         // для NUMBER
    std::string const_name;                  // для CONST
    std::vector<std::shared_ptr<Node>> kids; // аргументы/подузлы

    static std::shared_ptr<Node> num(double v);
    static std::shared_ptr<Node> cnst(const std::string &name);
    static std::shared_ptr<Node> unary(const std::string &o, std::shared_ptr<Node> a);
    static std::shared_ptr<Node> binary(const std::string &o, std::shared_ptr<Node> a, std::shared_ptr<Node> b);
    static std::shared_ptr<Node> call(const std::string &name, std::vector<std::shared_ptr<Node>> args);
};

std::shared_ptr<Node> parsing_to_ast(const std::vector<Token> &tokens);
std::string executing(const std::shared_ptr<Node> &ast);