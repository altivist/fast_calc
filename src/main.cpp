// просто все инклуды в одном
#include <bits/stdc++.h>

using namespace std;

// Набор токенов, на которые должна функция lexing() разложить пример.
// Нужно добавить основные операции
enum class TokType
{
    NUM,         // число (например: 3.14, 571)
    FUNC,        // какая-либо функция (sin, cos, sqrt, lg). Подбор функции для решения должен происходить по string text
    OP,          // оператор (+, -, *, /, ^)
    LPAREN,      // левая скобка '('
    RPAREN,      // правая скобка ')'
    COMMA,       // запятая ','
    PERIOD,      // точка '.'
    POSTFIX_DEG, // апостроф "'" - обозначение градусов
};

struct Token
{
    TokType type;       // тип токена
    std::string text;   // оригинальный текст (как во вводе)
    double value = 0.0; // значение, если это число

    Token(TokType t, std::string txt)
        : type(t), text(std::move(txt)) {}

    // если токен оказался числом
    Token(double num)
        : type(TokType::NUM), value(num), text(std::to_string(num)) {}
};

// функция принимает на вход пользовательский пример
// Функция должна вернуть дин. массив токенов
vector<Token> lexing(const string &input)
{
}

void show_menu()
{
    std::cout << "Вас приветствует fast_calc!" << std::endl;
    std::cout << "Введите пример для решения или 'q' для выхода из программы: ";
}

int main()
{
    // ускорим ввод/вывод, отключив printf и scanf
    ios::sync_with_stdio(false);

    show_menu();

    string input;
    getline(cin, input);

    try
    {
        vector<Token> tokens = lexing(input);

        // для отладки, потом уберу
        cout << "DEBUG 1: ";
        for (const auto &token : tokens)
        {
            cout << "Тип: " << static_cast<int>(token.t)
                 << "\tТекст: \"" << token.text << "\"\n";
        }

        // потом
        // auto ast = parsing_to_ast(tokens);
        // string result = executing(ast);
        // print_result(result);

        // auto ast = parsing_to_ast(tokens);
        // string result = executing(ast);
        // print_result(result);
    }
    catch (const exception &e)
    {
        cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
