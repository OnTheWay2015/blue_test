


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





