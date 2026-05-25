#include <iostream>
#include <string>
#include "lexer.h"
#include "parser.h"
#include "ast.h"
#include "vm.h"

using namespace std;

int main() {
    // 测试用Lua代码
    string luaCode = R"(
        a = 10 + 20 * 2;
        b = a - 5;
        100 / 4;
    )";

    try {
        // 1. 词法分析
        cout << "[1] 词法分析中..." << endl;
        Lexer lexer(luaCode);

        // 2. 语法分析 → 生成AST
        cout << "[2] 语法分析生成AST..." << endl;
        Parser parser(lexer);
        unique_ptr<BlockNode> ast = parser.parseProgram();

        // 3. 虚拟机执行AST
        cout << "[3] 执行程序..." << endl;
        VM vm;
        vm.run(ast.get());
        vm.dumpVars();
    }
    catch (const exception& e) {
        cerr << "错误: " << e.what() << endl;
        return 1;
    }

    return 0;
}