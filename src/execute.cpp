// src/execute.cpp
// Выполняет Хирвонен Матвей и Ефимов Игорь

string executing(const std::shared_ptr<Node> &ast)
{
    double v = eval(ast);
    return format_number(v);
}
