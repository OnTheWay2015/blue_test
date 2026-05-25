#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <cctype>
#include <stdexcept>

// Token类型
enum class TokenType {
    EOF_TOKEN,
    NUMBER,
    STRING,
    NAME,
    PLUS, MINUS, MUL, DIV,
    ASSIGN, // =
    LPAREN, RPAREN, // ( )
    SEMI // ;
};

// Token结构体
struct Token {
    TokenType type;
    std::string text;
    double numValue;

    Token(TokenType t = TokenType::EOF_TOKEN, const std::string& tx = "", double nv = 0.0)
        : type(t), text(tx), numValue(nv) {}
};

// 词法分析器
class Lexer {
private:
    std::string input;
    int pos = 0;
    char currentChar;

    void nextChar() {
        pos++;
        if (pos >= input.size()) currentChar = '\0';
        else currentChar = input[pos];
    }

    void skipWhitespace() {
        while (currentChar != '\0' && isspace(currentChar)) nextChar();
    }

public:
    Lexer(const std::string& in) : input(in) {
        if (in.empty()) currentChar = '\0';
        else currentChar = in[0];
    }

    // 获取下一个Token（核心方法）
    Token nextToken() {
        skipWhitespace();
        if (currentChar == '\0') return Token(TokenType::EOF_TOKEN);

        // 数字
        if (isdigit(currentChar) || currentChar == '.') {
            std::string numStr;
            while (isdigit(currentChar) || currentChar == '.') {
                numStr += currentChar;
                nextChar();
            }
            return Token(TokenType::NUMBER, numStr, std::stod(numStr));
        }

        // 字符串
        if (currentChar == '"' || currentChar == '\'') {
            char quote = currentChar;
            nextChar();
            std::string str;
            while (currentChar != '\0' && currentChar != quote) {
                str += currentChar;
                nextChar();
            }
            if (currentChar != quote) throw std::runtime_error("未闭合字符串");
            nextChar();
            return Token(TokenType::STRING, str);
        }

        // 标识符/变量名
        if (isalpha(currentChar) || currentChar == '_') {
            std::string name;
            while (isalnum(currentChar) || currentChar == '_') {
                name += currentChar;
                nextChar();
            }
            return Token(TokenType::NAME, name);
        }

        // 符号
        char c = currentChar;
        nextChar();
        switch (c) {
            case '+': return Token(TokenType::PLUS, "+");
            case '-': return Token(TokenType::MINUS, "-");
            case '*': return Token(TokenType::MUL, "*");
            case '/': return Token(TokenType::DIV, "/");
            case '=': return Token(TokenType::ASSIGN, "=");
            case '(': return Token(TokenType::LPAREN, "(");
            case ')': return Token(TokenType::RPAREN, ")");
            case ';': return Token(TokenType::SEMI, ";");
            default: throw std::runtime_error("未知字符: " + std::string(1, c));
        }
    }
};

#endif