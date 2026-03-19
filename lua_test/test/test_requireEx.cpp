#define LUA_OK 0

#include <unordered_map>
#include <string>



extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

LUALIB_API int luaopen_struct (lua_State *L);

}

using namespace std;

namespace TEST_REQUIRE_EX{

// 全局内存模块存储：键=模块名，值=模块的 Lua 代码字符串
std::unordered_map<std::string, std::string> g_memory_modules;

// ===================== 步骤1：实现内存模块加载器（C 函数，供 Lua 调用） =====================
// Lua C 函数签名：int func(lua_State *L)
// 入参：L 栈顶是模块名（string）
// 出参：返回 1 个值（编译后的模块加载函数）或 nil+错误信息
int memory_loader(lua_State *L) {
    // 1. 获取 require 传入的模块名（栈顶参数）
    const char *module_name = luaL_checkstring(L, 1);
    printf("内存加载器：尝试加载模块 %s\n", module_name);

    // 2. 检查内存中是否存在该模块
    auto it = g_memory_modules.find(module_name);
    if (it == g_memory_modules.end()) {
        // 内存中找不到，返回 nil + 错误信息（交给下一个加载器）
        lua_pushnil(L);
        lua_pushfstring(L,"模块[%s]未在内存中找到",module_name);
        return 2; // 返回 2 个值：nil + 错误信息
    }

    // 3. 从内存中获取模块代码字符串
    const std::string &module_code = it->second;

    // 4. 编译 Lua 代码字符串为可执行函数（兼容 Lua 5.1）
    // luaL_loadstring：编译字符串，成功则将函数压入栈，失败则压入错误信息
    int status = luaL_loadstring(L, module_code.c_str());
    if (status != LUA_OK) {
        // 编译失败，返回 nil + 错误信息
        const char *err = lua_tostring(L, -1);
        lua_pushnil(L);
        lua_pushfstring(L, "内存模块 %s 编译失败：%s", module_name, err);
        return 2;
    }

    // 5. 编译成功，返回加载函数（栈顶已压入编译后的函数）
    return 1;
}

// ===================== 步骤2：兼容 Lua 版本的加载器注入逻辑（核心修复） =====================
void inject_memory_loader(lua_State *L) {
    // 1. 获取 package 表
    lua_getglobal(L, "package");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        luaL_error(L, "找不到 package 表");
        return;
    }

    // 2. 兼容 Lua 5.1 (loaders) 和 Lua 5.2+ (searchers) 的核心逻辑
    // 先尝试获取 searchers（5.2+），获取不到则用 loaders（5.1）
    const char *searchers_key = "searchers";
    lua_getfield(L, -1, searchers_key);
    if (!lua_istable(L, -1)) {
        // searchers 不存在，改用 loaders（Lua 5.1）
        lua_pop(L, 1); // 弹出 nil
        searchers_key = "loaders";
        lua_getfield(L, -1, searchers_key);
        if (!lua_istable(L, -1)) {
            lua_pop(L, 2); // 弹出 loaders 查找结果 + package 表
            luaL_error(L, "找不到 package.searchers 或 package.loaders 表");
            return;
        }
    }

    // 3. 将自定义内存加载器（C 函数）注册为 Lua 函数，并插入到 searchers/loaders 表的第一个位置
    lua_pushcfunction(L, memory_loader); // 将 C 函数压入栈
    lua_rawseti(L, -2, 1); // 插入到表的索引 1 位置（优先执行）

    // 4. 清理栈（弹出 searchers/loaders 表 和 package 表）
    lua_pop(L, 2);
}

// ===================== 步骤3：主函数测试 =====================
int test() {
    // 1. 创建 Lua 状态机
    lua_State *L = luaL_newstate();
    if (!L) {
        fprintf(stderr, "创建 Lua 状态机失败\n");
        return 1;
    }
    luaL_openlibs(L); // 加载所有 Lua 标准库

    // 2. 初始化内存模块（模拟从内存加载的模块）
    g_memory_modules["math.utils"] = R"(
        local M = {}
        function M.add(a, b)
            return a + b
        end
        function M.mul(a, b)
            return a * b
        end
        return M
    )";
    g_memory_modules["calc"] = R"(
        local utils = require("math.utils")
        local M = {}
        function M.calculate(a, b)
            return utils.add(a, b), utils.mul(a, b)
        end
        return M
    )";

    // 3. 注入内存加载器到 Lua 的 require 流程（修复后版本）
    inject_memory_loader(L);

    // 4. 测试：在 Lua 中 require 内存模块
    printf("\n=== 测试 require 内存模块 ===\n");
    const char *test_code = R"(
        -- 加载 math.utils 模块
        local math_utils = require("math.utils")
        print("math_utils.add(10, 20) =", math_utils.add(10, 20))
        print("math_utils.mul(10, 20) =", math_utils.mul(10, 20))

        -- 加载依赖模块 calc
        local calc = require("calc")
        local add_res, mul_res = calc.calculate(5, 6)
        print("calc.calculate(5,6) =", add_res, mul_res)
    )";
    int status = luaL_dostring(L, test_code);
    if (status != LUA_OK) {
        fprintf(stderr, "Lua 执行错误：%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
    }

    // 5. 动态添加内存模块（运行时）
    printf("\n=== 动态添加内存模块 ===\n");
    g_memory_modules["dynamic.module"] = R"(
        return {
            say_hello = function(name)
                return "Hello, " .. name
            end
        }
    )";
    // 执行 Lua 代码加载动态添加的模块
    luaL_dostring(L, R"(
        local dynamic_mod = require("dynamic.module")
        print(dynamic_mod.say_hello("Lua/C++"))
    )");

    // 6. 销毁 Lua 状态机
    lua_close(L);
    return 0;
}

}
