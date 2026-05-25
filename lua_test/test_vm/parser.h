#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "ast.h"
#include <memory>
#include <vector>

class Parser {
private:
    Lexer lexer;
    Token currentToken;

    void eat(TokenType expected) {
        if (currentToken.type != expected) {
            throw std::runtime_error("语法错误：期望 " + std::to_string((int)expected));
        }
        currentToken = lexer.nextToken();
    }

    // 表达式解析（优先级：乘除 > 加减）
    std::unique_ptr<AstNode> parseExpression() {
        auto node = parseTerm();
        while (currentToken.type == TokenType::PLUS || currentToken.type == TokenType::MINUS) {
            Token op = currentToken;
            if (op.type == TokenType::PLUS) {
                eat(TokenType::PLUS);
                node = std::make_unique<BinOpNode>(std::move(node), parseTerm(), BinOp::ADD);
            } else {
                eat(TokenType::MINUS);
                node = std::make_unique<BinOpNode>(std::move(node), parseTerm(), BinOp::SUB);
            }
        }
        return node;
    }

    std::unique_ptr<AstNode> parseTerm() {
        auto node = parseFactor();
        while (currentToken.type == TokenType::MUL || currentToken.type == TokenType::DIV) {
            Token op = currentToken;
            if (op.type == TokenType::MUL) {
                eat(TokenType::MUL);
                node = std::make_unique<BinOpNode>(std::move(node), parseFactor(), BinOp::MUL);
            } else {
                eat(TokenType::DIV);
                node = std::make_unique<BinOpNode>(std::move(node), parseFactor(), BinOp::DIV);
            }
        }
        return node;
    }

    std::unique_ptr<AstNode> parseFactor() {
        Token token = currentToken;
        switch (token.type) {
            case TokenType::NUMBER: {
                eat(TokenType::NUMBER);
                return std::make_unique<NumberNode>(token.numValue);
            }
            case TokenType::STRING: {
                eat(TokenType::STRING);
                return std::make_unique<StringNode>(token.text);
            }
            case TokenType::NAME: {
                eat(TokenType::NAME);
                return std::make_unique<NameNode>(token.text);
            }
            case TokenType::LPAREN: {
                eat(TokenType::LPAREN);
                auto node = parseExpression();
                eat(TokenType::RPAREN);
                return node;
            }
            default: throw std::runtime_error("语法错误：无法解析因子");
        }
    }

    // 赋值语句: name = expr;
    std::unique_ptr<AstNode> parseAssignment() {
        auto nameNode = std::make_unique<NameNode>(currentToken.text);
        eat(TokenType::NAME);
        eat(TokenType::ASSIGN);
        auto expr = parseExpression();
        eat(TokenType::SEMI);
        return std::make_unique<AssignNode>(std::move(nameNode), std::move(expr));
    }

    // 语句
    std::unique_ptr<AstNode> parseStatement() {
        if (currentToken.type == TokenType::NAME) {
            // 预判是否是赋值
            Token peek = currentToken;
            (void)lexer.nextToken();
            if (currentToken.type == TokenType::ASSIGN) {
                currentToken = peek;
                return parseAssignment();
            }
            currentToken = peek;
        }
        auto expr = parseExpression();
        eat(TokenType::SEMI);
        return std::make_unique<ExprStatementNode>(std::move(expr));
    }

public:
    Parser(Lexer lex) : lexer(std::move(lex)) {
        currentToken = this->lexer.nextToken();
    }

    // 解析整个程序，返回AST根节点
    std::unique_ptr<BlockNode> parseProgram() {
        auto block = std::make_unique<BlockNode>();
        while (currentToken.type != TokenType::EOF_TOKEN) {
            block->addStatement(parseStatement());
        }
        return block;
    }
};

#endif