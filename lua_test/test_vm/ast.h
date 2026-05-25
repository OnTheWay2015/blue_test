#ifndef AST_H
#define AST_H

#include <string>
#include <vector>
#include <memory>

// AST节点类型
enum class AstNodeType {
    NUMBER,
    STRING,
    NAME,
    BINOP,  // 二元运算 + - * /
    ASSIGN, // 赋值 a = 1
    BLOCK,  // 代码块
    EXPR_STATEMENT // 表达式语句
};

// 二元操作符类型
enum class BinOp {
    ADD, SUB, MUL, DIV
};

// AST节点基类
struct AstNode {
    AstNodeType type;
    AstNode(AstNodeType t) : type(t) {}
    virtual ~AstNode() = default;
};

// 数字节点
struct NumberNode : AstNode {
    double value;
    NumberNode(double v) : AstNode(AstNodeType::NUMBER), value(v) {}
};

// 字符串节点
struct StringNode : AstNode {
    std::string value;
    StringNode(const std::string& v) : AstNode(AstNodeType::STRING), value(v) {}
};

// 变量名节点
struct NameNode : AstNode {
    std::string name;
    NameNode(const std::string& n) : AstNode(AstNodeType::NAME), name(n) {}
};

// 二元运算节点
struct BinOpNode : AstNode {
    std::unique_ptr<AstNode> left, right;
    BinOp op;
    BinOpNode(std::unique_ptr<AstNode> l, std::unique_ptr<AstNode> r, BinOp o)
        : AstNode(AstNodeType::BINOP), left(std::move(l)), right(std::move(r)), op(o) {}
};

// 赋值节点
struct AssignNode : AstNode {
    std::unique_ptr<NameNode> name;
    std::unique_ptr<AstNode> expr;
    AssignNode(std::unique_ptr<NameNode> n, std::unique_ptr<AstNode> e)
        : AstNode(AstNodeType::ASSIGN), name(std::move(n)), expr(std::move(e)) {}
};

// 语句块节点
struct BlockNode : AstNode {
    std::vector<std::unique_ptr<AstNode>> statements;
    BlockNode() : AstNode(AstNodeType::BLOCK) {}
    void addStatement(std::unique_ptr<AstNode> s) {
        statements.push_back(std::move(s));
    }
};

// 表达式语句
struct ExprStatementNode : AstNode {
    std::unique_ptr<AstNode> expr;
    ExprStatementNode(std::unique_ptr<AstNode> e)
        : AstNode(AstNodeType::EXPR_STATEMENT), expr(std::move(e)) {}
};

#endif