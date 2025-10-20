// src/calc.cpp
// Выполняет Тычина Ян
#include <iostream>

#include "AST.hpp"

using std::cout;

static void show_menu()
{
    cout << "Введите выражение без пробелов (до 128 символов).\n";
}

static void init()
{
    std::ios::sync_with_stdio(false);
    show_menu();
}

static void debug1(const std::vector<Token> &tokens)
{
    cout << "\nDEBUG 1: \n";
    for (const auto &token : tokens)
    {
        cout << "Тип: " << static_cast<int>(token.type)
             << "\tТекст: \"" << token.text << "\""
             << "\tЗначение: " << token.value << "\n";
    }
}

static void debug2_rec(const std::shared_ptr<Node> &node, int depth = 0)
{
    if (!node)
        return;
    for (int i = 0; i < depth; ++i)
        cout << "  ";
    cout << "Тип узла: " << static_cast<int>(node->type)
         << "\tОперация/Имя: \"" << node->op << "\""
         << "\tЗначение: " << node->number
         << "\tИмя константы: " << node->const_name << "\n";
    for (const auto &kid : node->kids)
    {
        debug2_rec(kid, depth + 1);
    }
}

static void debug2(const std::shared_ptr<Node> &node)
{
    cout << "\nDEBUG 2: \n";
    debug2_rec(node, 0);
    cout << "\n\n";
}

double eval_func(const std::string &input)
{

    // init();
    // std::string input;

    // if (!std::getline(std::cin, input))
    // {
    //     throw std::runtime_error("Фатальная ошибка: не удалось прочитать ввод");
    // }

    // std::vector<Token> tokens = lexing(input);
    // // debug1(tokens);
    // std::shared_ptr<Node> ast = parsing_to_ast(tokens);
    // // debug2(ast);
    // std::string output = executing(ast);
    // return std::stod(output);
    return 123.123;
}
