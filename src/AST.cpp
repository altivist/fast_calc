shared_ptr<Node> Node::num(double v)
{
    auto n = make_shared<Node>();
    n->type = NodeType::NUMBER;
    n->number = v;
    return n;
}

shared_ptr<Node> Node::cnst(const string &name)
{
    auto n = make_shared<Node>();
    n->type = NodeType::CONST;
    n->const_name = name;
    return n;
}
shared_ptr<Node> Node::unary(const string &o, shared_ptr<Node> a)
{
    auto n = make_shared<Node>();
    n->type = NodeType::UNARY;
    n->op = o;
    n->kids = {move(a)};
    return n;
}
shared_ptr<Node> Node::binary(const string &o, shared_ptr<Node> a, shared_ptr<Node> b)
{
    auto n = make_shared<Node>();
    n->type = NodeType::BINARY;
    n->op = o;
    n->kids = {move(a), move(b)};
    return n;
}
shared_ptr<Node> Node::call(const string &name, vector<shared_ptr<Node>> args)
{
    auto n = make_shared<Node>();
    n->type = NodeType::CALL;
    n->op = name;
    n->kids = move(args);
    return n;
}

shared_ptr<Node> parsing_to_ast(const vector<Token> &tokens)
{
    Parser p(tokens);
    return p.parse();
}
