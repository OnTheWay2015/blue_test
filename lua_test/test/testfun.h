#pragma once

#include <cstring>
#include <type_traits>
#include <utility>
#include <tuple>
#include <cstdio>

// 默认是 c++98 追加编译参数 /Zc:__cplusplus 
// 修复VS2022 __cplusplus宏显示错误的问题（兼容MSVC和其他编译器）
#if defined(_MSC_VER)
#if _MSVC_LANG < 201103L
#error "This code requires C++11 or later (set C++11+ in project properties)"
#endif
#else
#if __cplusplus < 201103L
#error "This code requires C++11 or later (compile with -std=c++11)"
#endif
#endif

// 引入Lua C API（请根据实际路径调整）
extern "C" {
#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"
}

// ===================== 1. 核心工具命名空间（避免和标准库重名） =====================
namespace lua_bind_detail {
    // -------------------- 1.1 手动实现C++11版index_sequence --------------------
    template <std::size_t... Ints>
    struct index_sequence {};

    template <std::size_t N, std::size_t... Ints>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ints...> {
        typedef typename make_index_sequence<N - 1, N - 1, Ints...>::type type;
    };

    template <std::size_t... Ints>
    struct make_index_sequence<0, Ints...> : index_sequence<Ints...> {
        typedef index_sequence<Ints...> type;
    };

    // -------------------- 1.2 类型判断工具（放入命名空间，避免冲突） --------------------
    // 基础模板：判断是否为成员函数指针
    template<typename T>
    struct is_member_function_pointer : std::false_type {};

    // 特化：成员函数指针
    template<typename Ret, typename Class, typename... Args>
    struct is_member_function_pointer<Ret(Class::*)(Args...)> : std::true_type {};

    // 基础模板：判断是否为普通函数指针
    template<typename T>
    struct is_function_pointer : std::false_type {};

    // 特化：普通函数指针
    template<typename Ret, typename... Args>
    struct is_function_pointer<Ret(*)(Args...)> : std::true_type {};
} // namespace lua_bind_detail

// ===================== 2. Lua类型转换体系 =====================
template<typename T>
struct LuaTypeTraits;

template<>
struct LuaTypeTraits<int> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static int get(lua_State* L, int idx) { return lua_tointeger(L, idx); }
    static const char* name() { return "integer"; }
};

template<>
struct LuaTypeTraits<float> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static float get(lua_State* L, int idx) { return static_cast<float>(lua_tonumber(L, idx)); }
    static const char* name() { return "float"; }
};

template<>
struct LuaTypeTraits<double> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static double get(lua_State* L, int idx) { return lua_tonumber(L, idx); }
    static const char* name() { return "double"; }
};

template<>
struct LuaTypeTraits<const char*> {
    static const int LuaType = LUA_TSTRING;
    static int check(lua_State* L, int idx) { return lua_isstring(L, idx); }
    static const char* get(lua_State* L, int idx) { return lua_tostring(L, idx); }
    static const char* name() { return "string"; }
};

template<>
struct LuaTypeTraits<bool> {
    static const int LuaType = LUA_TBOOLEAN;
    static int check(lua_State* L, int idx) { return lua_isboolean(L, idx); }
    static bool get(lua_State* L, int idx) { return lua_toboolean(L, idx) != 0; }
    static const char* name() { return "boolean"; }
};

// 读取单个参数（带类型检查）
template<typename T>
T read_arg(lua_State* L, int idx) {
    if (!LuaTypeTraits<T>::check(L, idx)) {
        char err_msg[128];
        snprintf(err_msg, sizeof(err_msg),
            "argument %d: expected %s, got %s",
            idx, LuaTypeTraits<T>::name(), lua_typename(L, lua_type(L, idx)));
        lua_pushstring(L, err_msg);
        lua_error(L);
    }
    return LuaTypeTraits<T>::get(L, idx);
}

// ===================== 3. 递归读取参数包 =====================
template<typename... Args>
struct ArgsReader;

template<>
struct ArgsReader<> {
    static std::tuple<> read(lua_State* /*L*/, int /*idx*/) {
        return std::tuple<>();
    }
};

template<typename T, typename... Rest>
struct ArgsReader<T, Rest...> {
    static std::tuple<T, Rest...> read(lua_State* L, int idx) {
        T arg = read_arg<T>(L, idx);
        auto rest_args = ArgsReader<Rest...>::read(L, idx + 1);
        return std::tuple_cat(std::make_tuple(arg), rest_args);
    }
};

template<typename... Args>
std::tuple<Args...> read_args(lua_State* L) {
    const std::size_t expected = sizeof...(Args);
    const int got = lua_gettop(L);
    if (expected != static_cast<std::size_t>(got)) {
        char err_msg[128];
        snprintf(err_msg, sizeof(err_msg),
            "wrong number of arguments: expected %zu, got %d",
            expected, got);
        lua_pushstring(L, err_msg);
        lua_error(L);
    }
    return ArgsReader<Args...>::read(L, 1);
}

// ===================== 4. 函数调用与返回值处理 =====================
// 推送返回值
template<typename T>
struct ReturnPusher;

template<>
struct ReturnPusher<int> {
    static void push(lua_State* L, int value) { lua_pushinteger(L, value); }
};

template<>
struct ReturnPusher<float> {
    static void push(lua_State* L, float value) { lua_pushnumber(L, value); }
};

template<>
struct ReturnPusher<double> {
    static void push(lua_State* L, double value) { lua_pushnumber(L, value); }
};

template<>
struct ReturnPusher<const char*> {
    static void push(lua_State* L, const char* value) { lua_pushstring(L, value); }
};

template<>
struct ReturnPusher<bool> {
    static void push(lua_State* L, bool value) { lua_pushboolean(L, value ? 1 : 0); }
};

template<>
struct ReturnPusher<void> {
    static void push(lua_State* /*L*/) {}
};

// -------------------- 4.1 普通函数调用器 --------------------
// 辅助：调用无返回值普通函数
template<typename Func, typename... Args, std::size_t... Is>
void call_func_void(Func func, const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    func(std::get<Is>(args)...);
}

// 辅助：调用有返回值普通函数
template<typename Ret, typename Func, typename... Args, std::size_t... Is>
Ret call_func_ret(Func func, const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    return func(std::get<Is>(args)...);
}

// 普通函数Caller
template<typename Func>
struct FuncCaller;

// 偏特化1：void返回值普通函数（用类型别名简化）
template<typename... Args>
struct FuncCaller<void(*)(Args...)> {
    typedef void(*FuncType)(Args...);
    static int call(lua_State* L, FuncType func) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        call_func_void(func, args, Indices());
        ReturnPusher<void>::push(L);
        return 0;
    }
};

// 偏特化2：非void返回值普通函数（用类型别名简化）
template<typename Ret, typename... Args>
struct FuncCaller<Ret(*)(Args...)> {
    typedef Ret(*FuncType)(Args...);
    static int call(lua_State* L, FuncType func) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        Ret result = call_func_ret<Ret>(func, args, Indices());
        ReturnPusher<Ret>::push(L, result);
        return 1;
    }
};

// -------------------- 4.2 成员函数调用器 --------------------
// 辅助：调用无返回值成员函数
template<typename Class, typename... Args, std::size_t... Is>
void call_memfunc_void(void* obj_ptr, void (Class::* func)(Args...), const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    Class* obj = static_cast<Class*>(obj_ptr);
    (obj->*func)(std::get<Is>(args)...);
}

// 辅助：调用有返回值成员函数
template<typename Ret, typename Class, typename... Args, std::size_t... Is>
Ret call_memfunc_ret(void* obj_ptr, Ret(Class::* func)(Args...), const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    Class* obj = static_cast<Class*>(obj_ptr);
    return (obj->*func)(std::get<Is>(args)...);
}

// 成员函数Caller
template<typename Func>
struct MemFuncCaller;

// 偏特化1：void返回值成员函数（用类型别名简化）
template<typename Class, typename... Args>
struct MemFuncCaller<void (Class::*)(Args...)> {
    typedef void (Class::* MemFuncType)(Args...);
    static int call(lua_State* L, void* obj_ptr, MemFuncType func) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        call_memfunc_void(obj_ptr, func, args, Indices());
        ReturnPusher<void>::push(L);
        return 0;
    }
};

// 偏特化2：非void返回值成员函数（用类型别名简化）
template<typename Ret, typename Class, typename... Args>
struct MemFuncCaller<Ret(Class::*)(Args...)> {
    typedef Ret(Class::* MemFuncType)(Args...);
    static int call(lua_State* L, void* obj_ptr, MemFuncType func) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        Ret result = call_memfunc_ret<Ret>(obj_ptr, func, args, Indices());
        ReturnPusher<Ret>::push(L, result);
        return 1;
    }
};

// ===================== 5. 通用函数包装器（修复VS2022语法解析bug） =====================
template<typename Func>
struct FunctionWrapper;

// 5.1 普通函数包装器（核心修复：类型别名+拆分转换）
template<typename Ret, typename... Args>
struct FunctionWrapper<Ret(*)(Args...)> {
    // 类型别名简化函数指针类型
    typedef Ret(*FuncType)(Args...);

    static int call(lua_State* L) {
        // 读取userdata
        void* ud = lua_touserdata(L, lua_upvalueindex(1));

        // 拆分转换步骤，避免编译器解析错误
        FuncType* func_ptr = static_cast<FuncType*>(ud);
        FuncType func = *func_ptr;

        // 调用Caller
        return FuncCaller<FuncType>::call(L, func);
    }
};

// 5.2 成员函数包装器（同样用类型别名简化）
template<typename Ret, typename Class, typename... Args>
struct FunctionWrapper<Ret(Class::*)(Args...)> {
    // 类型别名简化成员函数指针类型
    typedef Ret(Class::* MemFuncType)(Args...);

    static int call(lua_State* L) {
        // 读取userdata
        void* ud_raw = lua_touserdata(L, lua_upvalueindex(1));
        struct Data { void* obj; MemFuncType func; };
        Data* ud = static_cast<Data*>(ud_raw);

        // 调用Caller
        return MemFuncCaller<MemFuncType>::call(L, ud->obj, ud->func);
    }
};

// ===================== 6. 核心注册函数（支持普通函数/成员函数） =====================
// 6.1 普通函数注册
template<typename Func>
typename std::enable_if<lua_bind_detail::is_function_pointer<Func>::value>::type
_ToluaRegfuncs(lua_State* L, const char* name, Func func) {
    if (!lua_istable(L, -1)) {
        lua_pushstring(L, "target is not a Lua table");
        lua_error(L);
        return;
    }

    lua_pushstring(L, name);
    // 分配userdata存储函数指针
    void* ud = lua_newuserdata(L, sizeof(Func));
    *reinterpret_cast<Func*>(ud) = func;

    // 绑定包装函数
    lua_pushcclosure(L, &FunctionWrapper<Func>::call, 1);
    lua_rawset(L, -3);
}

// 6.2 成员函数注册
template<typename Func>
typename std::enable_if<lua_bind_detail::is_member_function_pointer<Func>::value>::type
_ToluaRegfuncs(lua_State* L, const char* name, void* obj_ptr, Func func) {
    if (!lua_istable(L, -1)) {
        lua_pushstring(L, "target is not a Lua table");
        lua_error(L);
        return;
    }

    lua_pushstring(L, name);
    // 分配userdata存储：对象指针 + 成员函数指针
    struct Data { void* obj; Func func; };
    Data* ud = static_cast<Data*>(lua_newuserdata(L, sizeof(Data)));
    ud->obj = obj_ptr;
    ud->func = func;

    // 绑定包装函数
    lua_pushcclosure(L, &FunctionWrapper<Func>::call, 1);
    lua_rawset(L, -3);
}

// ===================== 7. 辅助函数 =====================
namespace baseitem_detail {
    static void func0_impl() {
        printf("无参数函数调用\n");
    }

    static int func1_impl(int a) {
        return a * 2;
    }

    static const char* func2_impl(int a, const char* s, bool b) {
        static char buf[128];
        snprintf(buf, sizeof(buf), "a=%d, s=%s, b=%s", a, s, b ? "true" : "false");
        return buf;
    }

    static double func3_impl(float a, double b) {
        return a + b;
    }
} // namespace baseitem_detail

// ===================== 8. 类定义 =====================
class baseitem
{
public:
    baseitem() {}
    ~baseitem() {}
public:
    void ToluaRegfuncsStart(lua_State* L);
    void ToluaRegfuncsEnd(lua_State* L);

protected:
    int getInt() 
    { 
        return 123; 
    }
    void ToluaRegfuncs(lua_State* L);

public:
    void RegisterAll(lua_State* L) {
        // 注册普通函数
        _ToluaRegfuncs(L, "func0", baseitem_detail::func0_impl);
        _ToluaRegfuncs(L, "func1", baseitem_detail::func1_impl);
        _ToluaRegfuncs(L, "func2", baseitem_detail::func2_impl);
        _ToluaRegfuncs(L, "func3", baseitem_detail::func3_impl);

        // 注册成员函数
        _ToluaRegfuncs(L, "gggg", static_cast<void*>(this), &baseitem::getInt);
    }
};

// ===================== 9. 测试主函数（可选） =====================
/*
int main() {
    // 初始化Lua
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    // 创建Lua表
    lua_newtable(L);

    // 注册函数
    baseitem obj;
    obj.RegisterAll(L);

    // 设置全局表
    lua_setglobal(L, "my_table");

    // 调用测试
    luaL_dostring(L, "print(my_table.gggg())");    // 输出123
    luaL_dostring(L, "print(my_table.func1(10))"); // 输出20

    // 关闭Lua
    lua_close(L);
    return 0;
}
*/

