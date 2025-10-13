// просто все инклуды в одном
#include <bits/stdc++.h>

using namespace std;

// Выполняет Смирнов Даниил
// Задача: чтобы функция lexing выполняла свою работу (смотреть коммент функции)
// Можно использовать для этого всё, что угодно: (ООП, исключения и т.д.)

// Набор токенов, на которые должна функция lexing() разложить пример.
enum class TokType
{
    NUM,         // число (например: 3.14, 571)
    FUNC,        // какая-либо функция (sin, cos, sqrt, lg). Подбор функции для решения должен происходить по string text
    OP,          // оператор (+, -, *, /, ^)
    LPAREN,      // левая скобка '('
    RPAREN,      // правая скобка ')'
    COMMA,       // запятая ',', для функций с несколькими аргументами
    PERIOD,      // точка '.', служит разделителем чисел
    POSTFIX_DEG, // апостроф "'" - обозначение градусов
};

struct Token
{
    TokType type;       // тип токена
    std::string text;   // оригинальный текст (как во вводе)
    double value = 0.0; // значение, если это число

    Token(TokType t, string txt)
        : type(t), text(move(txt)) {}

    // если токен оказался числом
    Token(double num)
        : type(TokType::NUM), value(num), text(to_string(num)) {}
};

// функция принимает на вход пользовательский пример
// Функция должна вернуть дин. массив токенов
// функция может выбрасывать исключения
vector<Token> lexing(const string &input)
{
}

// Выполняет Иванов Константин
// Цель: чтобы функция parsing_to_ast выполняла свою работу (смотреть коммент функции)
// Можно использовать для этого всё, что угодно: (ООП, исключения и т.д.)

enum class NodeType
{
    NUMBER,   // числовое значение, лист
    OPERATOR, // унарный или бинарный оператор
    FUNCTION  // вызов функции, один или несколько аргументов
};

struct Node
{
    NodeType type;
    std::string value;                           // имя функции или оператора
    double num_value;                            // если число
    std::vector<std::shared_ptr<Node>> children; // подузлы

    Node(NodeType t, std::string v) : type(t), value(std::move(v)), num_value(0) {}
    Node(double val) : type(NodeType::NUMBER), num_value(val) {}

    bool isNumber() const { return type == NodeType::NUMBER; }
    bool isFunction() const { return type == NodeType::FUNCTION; }
    bool isOperator() const { return type == NodeType::OPERATOR; }
};

// Получает вектор токенов
// Возвращает указатель на главный узел
// Цель: получить из плоской последовательности токенов - дерево выражения (AST),
// где операции, функции и скобки отражают приоритеты и иерархию вычислений
shared_ptr<Node> parsing_to_ast(const vector<Token> &tokens)
{
    // Выполняет Иванов Константин
}

void show_menu()
{
    std::cout << "Вас приветствует fast_calc!" << std::endl;
    std::cout << "Введите пример для решения: ";
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