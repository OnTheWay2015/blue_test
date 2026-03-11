


/*
// 创建目标table（压入栈顶，此时栈深度=1）
lua_newtable(L); 

lua_setfield
lua_settable
*/


#include <iostream>
#include <sstream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;


#include <lua.hpp>
#include <iostream>
#include <string>

// 辅助函数：检查Lua错误
void check_lua_error(lua_State* L, int status) {
    if (status != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << std::endl;
        lua_pop(L, 1);
        exit(1);
    }
}

// 辅助函数：打印栈中指定位置的值（调试用）
void print_lua_value(lua_State* L, int idx) {
    switch (lua_type(L, idx)) {
    case LUA_TNIL:
        std::cout << "nil";
        break;
    case LUA_TNUMBER:
        // 区分整数和浮点数
        if (lua_isinteger(L, idx)) {
            std::cout << lua_tointeger(L, idx);
        }
        else {
            std::cout << lua_tonumber(L, idx);
        }
        break;
    case LUA_TSTRING:
        std::cout << "\"" << lua_tostring(L, idx) << "\"";
        break;
    case LUA_TBOOLEAN:
        std::cout << (lua_toboolean(L, idx) ? "true" : "false");
        break;
    case LUA_TTABLE:
        std::cout << "table: " << lua_topointer(L, idx);
        break;
    default:
        std::cout << lua_typename(L, lua_type(L, idx));
        break;
    }
}

int test_visit_table(){
    // 1. 初始化Lua环境
    lua_State* L = luaL_newstate();
    if (!L) {
        std::cerr << "Failed to create Lua state!" << std::endl;
        return 1;
    }
    luaL_openlibs(L);

    // 2. 加载并执行Lua脚本
    int status = luaL_dofile(L, "test.lua");
    check_lua_error(L, status);

    // 3. 调用函数获取table
    lua_getglobal(L, "get_mixed_table");
    status = lua_pcall(L, 0, 1, 0);
    check_lua_error(L, status);

    // 4. 检查是否为table
    if (!lua_istable(L, -1)) {
        std::cerr << "Error: get_mixed_table() did not return a table!" << std::endl;
        lua_close(L);
        return 1;
    }
    int table_idx = lua_gettop(L);  // 记录table在栈中的位置（避免后续操作偏移）

    // ===================== 方式1：通用遍历（支持所有类型键） =====================
    std::cout << "===== 通用遍历（所有键值对）=====\n";
    // 初始键压入nil，触发第一次lua_next,会弹出栈顶
    lua_pushnil(L);
    // 循环遍历：lua_next返回1表示有下一个键值对，0表示遍历结束
    while (lua_next(L, table_idx) != 0) { //lua_next 弹出栈顶的 key1（也就是上一轮剩下的 “键”）；
        // 此时栈结构：... table 键 值
        std::cout << "键: ";
        print_lua_value(L, -2);  // -2是键
        std::cout << " | 值: ";
        print_lua_value(L, -1);  // -1是值
        std::cout << std::endl;

        // 弹出值，保留键供下一次lua_next使用
        lua_pop(L, 1);
    }

    // ===================== 方式2：数组式遍历（仅数字索引） =====================
    std::cout << "\n===== 数组式遍历（仅数字键）=====\n";
    // 获取table长度（Lua 5.1+支持lua_rawlen，更高效）
    size_t table_len = lua_rawlen(L, table_idx);
    for (size_t i = 1; i <= table_len; ++i) {
        // 压入数字键，获取对应值
        lua_pushinteger(L, i);
        lua_gettable(L, table_idx);

        std::cout << "索引 " << i << ": ";
        print_lua_value(L, -1);
        std::cout << std::endl;

        // 弹出值，清理栈
        lua_pop(L, 1);
    }

    // 5. 清理栈并释放资源
    lua_pop(L, 1);  // 弹出table
    lua_close(L);

    return 0;
}


// 自定义__index元方法：当访问table不存在的键时触发
static int l_metatable_index(lua_State* L) {
    // 栈入参：1=目标table，2=访问的key
    const char* key = lua_tostring(L, 2);
    if (key == NULL) {
        lua_pushnil(L);
        return 1;
    }

    // 模拟：访问不存在的key时返回默认值
    std::cout << "R(触发__index元方法，访问的key：)" << key << std::endl;
    if (strcmp(key, "default") == 0) {
        lua_pushstring(L, "R(这是默认值)");
    }
    else {
        lua_pushstring(L, "key不存在，返回默认提示");
    }
    return 1; // 返回1个值给Lua
}

// 为Lua栈中指定位置的table设置元表
void set_table_metatable(lua_State* L, int table_idx) {
    // 1. 创建空的元表（压入栈顶）
    lua_newtable(L);
    std::cout << "R(创建元表后栈深度：)" << lua_gettop(L) << std::endl; // 原栈深度+1

    // 2. 为元表绑定__index元方法
    lua_pushcfunction(L, l_metatable_index); // 压入C函数作为元方法
    lua_setfield(L, -2, "__index"); // 元表.__index = l_metatable_index
    std::cout << "R(绑定__index后栈深度：)" << lua_gettop(L) << std::endl; // 栈深度不变（弹函数，设字段）

    // 3. 核心操作：为table_idx位置的table设置元表
    // 此时栈顶是元表，调用lua_setmetatable后弹出元表，目标table保留
    lua_setmetatable(L, table_idx);
    std::cout << "R(设置元表后栈深度：)" << lua_gettop(L) << std::endl; // 栈深度-1（弹出元表）
}


void test_table() {
    // 1. 创建Lua虚拟机
    lua_State* L = luaL_newstate();
    luaL_openlibs(L); // 打开Lua标准库

    // 2. 创建目标table（压入栈顶，此时栈深度=1）
    lua_newtable(L);
    std::cout << "R(创建目标table后栈深度：)" << lua_gettop(L) << std::endl; // 输出：1

    // 3. 为目标table设置元表（table在栈顶，索引为-1/1）
    set_table_metatable(L, -1);

    // 4. 测试元表是否生效：访问table的不存在key
    std::cout << "\n===== 测试元表效果 =====" << std::endl;
    // 压入要访问的key："test_key"
    lua_pushstring(L, "test_key");
    // 读取table["test_key"]（会触发__index元方法）
    lua_gettable(L, -2);
    // 输出返回值
    const char* result = lua_tostring(L, -1);
    std::cout << "访问table[\"test_key\"]的结果：" << result << std::endl;
    lua_pop(L, 1); // 弹出返回值

    // 5. 访问元表绑定的默认key
    lua_pushstring(L, "default");
    lua_gettable(L, -2);
    result = lua_tostring(L, -1);
    std::cout << "访问table[\"default\"]的结果：" << result << std::endl;
    lua_pop(L, 1);

    // 6. 清理Lua虚拟机
    lua_close(L);
}





