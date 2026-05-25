#ifndef VM_H
#define VM_H

#include "ast.h"
#include <unordered_map>
#include <stdexcept>
#include <iostream>

// 虚拟机：执行AST
class VM {
private:
    std::unordered_map<std::string, double> variables; // 变量表

    double evaluate(AstNode* node) {
        switch (node->type) {
            case AstNodeType::NUMBER:
                return static_cast<NumberNode*>(node)->value;
            case AstNodeType::NAME: {
                std::string name = static_cast<NameNode*>(node)->name;
                if (!variables.count(name)) throw std::runtime_error("变量未定义: " + name);
                return variables[name];
            }
            case AstNodeType::BINOP: {
                auto bin = static_cast<BinOpNode*>(node);
                double l = evaluate(bin->left.get());
                double r = evaluate(bin->right.get());
                switch (bin->op) {
                    case BinOp::ADD: return l + r;
                    case BinOp::SUB: return l - r;
                    case BinOp::MUL: return l * r;
                    case BinOp::DIV: return l / r;
                    default: throw std::runtime_error("未知操作符");
                }
            }
            case AstNodeType::EXPR_STATEMENT:
                return evaluate(static_cast<ExprStatementNode*>(node)->expr.get());
            default:
                throw std::runtime_error("不支持的节点类型");
        }
    }

    void execute(AstNode* node) {
        switch (node->type) {
            case AstNodeType::BLOCK: {
                auto block = static_cast<BlockNode*>(node);
                for (auto& s : block->statements) execute(s.get());
                break;
            }
            case AstNodeType::ASSIGN: {
                auto assign = static_cast<AssignNode*>(node);
                std::string name = assign->name->name;
                double val = evaluate(assign->expr.get());
                variables[name] = val;
                std::cout << "[VM] 赋值: " << name << " = " << val << std::endl;
                break;
            }
            case AstNodeType::EXPR_STATEMENT: {
                double res = evaluate(node);
                std::cout << "[VM] 表达式结果: " << res << std::endl;
                break;
            }
            default:
                throw std::runtime_error("无法执行节点");
        }
    }

public:
    void run(BlockNode* root) {
        std::cout << "[VM] 开始执行 Lua 脚本..." << std::endl;
        execute(root);
        std::cout << "[VM] 执行完成" << std::endl;
    }

    // 打印所有变量
    void dumpVars() {
        std::cout << "\n[VM] 变量表:" << std::endl;
        for (auto& [k, v] : variables) {
            std::cout << "  " << k << " = " << v << std::endl;
        }
    }
};

#endif