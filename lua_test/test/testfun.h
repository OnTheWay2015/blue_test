#pragma once

/*

extern "C" { 

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

template<typename CALLFUN>
int _wrapfunc(lua_State* L, CALLFUN func)
{

	return 0;
}


typedef void(*FF)();
template<>
int _wrapfunc(lua_State* L, FF func)
{
	(*func)();
	return 0;
}




template<typename CALLFUN>
int wrapfunc(lua_State* L)
{
	void* d = lua_touserdata(L,lua_upvalueindex(1));

	auto x = d;
	auto func = (CALLFUN*)(d); 
	(*func)();
	return 1;
}


template<typename CALL_FUN>
void _ToluaRegfuncs(lua_State* L, const char* name, CALL_FUN func)
{
	if (!lua_istable(L, -1))
	{
		//assert(true);
		return;
	}
	
	lua_pushstring(L,name);
	void* d = lua_newuserdata(L,sizeof(func));
	memcpy(d,&func,sizeof(func));

	lua_pushcclosure(L, [](lua_State* L) {
		return wrapfunc<CALL_FUN>(L);
		},1);
	
	lua_rawset(L,-3);


}


class baseitem
{
public:
	baseitem() {}
	~baseitem() {}
public:
	void ToluaMergefunc(lua_State* L);
	void ToluaRegfuncsEnd(lua_State* L);

protected:
	int getInt();
protected:
	virtual void ToluaRegfuncs(lua_State* L);
};




*/

#pragma once

#include <cstring>
#include <type_traits>
#include <utility>
#include <tuple>
#include <cstdio>

// 修复VS2022 __cplusplus宏显示错误的问题（兼容MSVC和其他编译器）
#if defined(_MSC_VER)
    // VS专用：_MSVC_LANG标识实际C++标准版本
#if _MSVC_LANG < 201103L
#error "This code requires C++11 or later (set C++11+ in project properties)"
#endif
#else
    // GCC/Clang使用标准__cplusplus宏
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

// ===================== 1. 手动实现C++11版index_sequence（修复命名冲突+添加type成员） =====================
namespace lua_bind_detail {
    // 基础索引序列模板
    template <std::size_t... Ints>
    struct index_sequence {};

    // 索引序列生成器（递归）
    template <std::size_t N, std::size_t... Ints>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ints...> {
        // 关键：添加type嵌套类型，继承父类的type
        typedef typename make_index_sequence<N - 1, N - 1, Ints...>::type type;
    };

    // 终止条件：特化版本
    template <std::size_t... Ints>
    struct make_index_sequence<0, Ints...> : index_sequence<Ints...> {
        // 关键：定义最终的type成员
        typedef index_sequence<Ints...> type;
    };
} // namespace lua_bind_detail

// ===================== 2. Lua类型转换体系（C++11兼容） =====================
template<typename T>
struct LuaTypeTraits;

// int类型特化
template<>
struct LuaTypeTraits<int> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static int get(lua_State* L, int idx) { return lua_tointeger(L, idx); }
    static const char* name() { return "integer"; }
};

// float类型特化
template<>
struct LuaTypeTraits<float> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static float get(lua_State* L, int idx) { return static_cast<float>(lua_tonumber(L, idx)); }
    static const char* name() { return "float"; }
};

// double类型特化
template<>
struct LuaTypeTraits<double> {
    static const int LuaType = LUA_TNUMBER;
    static int check(lua_State* L, int idx) { return lua_isnumber(L, idx); }
    static double get(lua_State* L, int idx) { return lua_tonumber(L, idx); }
    static const char* name() { return "double"; }
};

// const char*类型特化
template<>
struct LuaTypeTraits<const char*> {
    static const int LuaType = LUA_TSTRING;
    static int check(lua_State* L, int idx) { return lua_isstring(L, idx); }
    static const char* get(lua_State* L, int idx) { return lua_tostring(L, idx); }
    static const char* name() { return "string"; }
};

// bool类型特化
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

// ===================== 3. 递归读取参数包（C++11） =====================
template<typename... Args>
struct ArgsReader;

// 终止条件：空参数包
template<>
struct ArgsReader<> {
    static std::tuple<> read(lua_State* /*L*/, int /*idx*/) {
        return std::tuple<>();
    }
};

// 递归读取参数包
template<typename T, typename... Rest>
struct ArgsReader<T, Rest...> {
    static std::tuple<T, Rest...> read(lua_State* L, int idx) {
        T arg = read_arg<T>(L, idx);
        auto rest_args = ArgsReader<Rest...>::read(L, idx + 1);
        return std::tuple_cat(std::make_tuple(arg), rest_args);
    }
};

// 对外接口：读取所有参数
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

// ===================== 4. 函数调用与返回值处理（修复所有语法错误） =====================
// 辅助：调用无返回值函数
template<typename Func, typename... Args, std::size_t... Is>
void call_func_void(Func func, const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    func(std::get<Is>(args)...);
}

// 辅助：调用有返回值函数
template<typename Ret, typename Func, typename... Args, std::size_t... Is>
Ret call_func_ret(Func func, const std::tuple<Args...>& args, lua_bind_detail::index_sequence<Is...>) {
    return func(std::get<Is>(args)...);
}

// 推送返回值（修复void参数错误）
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
    static void push(lua_State* /*L*/) {} // 无参数，修复e0526错误
};

// 核心修复：FuncCaller主模板（仅接收单个函数指针类型参数）
template<typename Func>
struct FuncCaller;

// 偏特化1：void返回值的函数指针
template<typename... Args>
struct FuncCaller<void(*)(Args...)> {
    static int call(lua_State* L, void(*func)(Args...)) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        call_func_void(func, args, Indices());
        ReturnPusher<void>::push(L);
        return 0;
    }
};

// 偏特化2：非void返回值的函数指针
template<typename Ret, typename... Args>
struct FuncCaller<Ret(*)(Args...)> {
    static int call(lua_State* L, Ret(*func)(Args...)) {
        auto args = read_args<Args...>(L);
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;
        Ret result = call_func_ret<Ret>(func, args, Indices());
        ReturnPusher<Ret>::push(L, result);
        return 1;
    }
};

// 通用函数包装器
template<typename Func>
struct FunctionWrapper;

// 特化：函数指针（返回值+参数包）
template<typename Ret, typename... Args>
struct FunctionWrapper<Ret(*)(Args...)> {
    static int call(lua_State* L, Ret(*func)(Args...)) {
        return FuncCaller<Ret(*)(Args...)>::call(L, func);
    }
};

// ===================== 5. 核心包装与注册函数 =====================
template<typename CALLFUN>
int _wrapfunc(lua_State* L, CALLFUN func) {
    return FunctionWrapper<CALLFUN>::call(L, func);
}

// 核心包装函数
template<typename CALL_FUN>
int wrapfunc(lua_State* L) {
    void* d = lua_touserdata(L, lua_upvalueindex(1));
    if (!d) {
        lua_pushstring(L, "invalid function pointer (null userdata)");
        lua_error(L);
        return 0;
    }

    CALL_FUN* func_ptr = reinterpret_cast<CALL_FUN*>(d);
    return _wrapfunc(L, *func_ptr);
}

// 注册函数到Lua表
template<typename CALL_FUN>
void _ToluaRegfuncs(lua_State* L, const char* name, CALL_FUN func) {
    if (!lua_istable(L, -1)) {
        lua_pushstring(L, "target is not a Lua table");
        lua_error(L);
        return;
    }

    lua_pushstring(L, name);
    void* d = lua_newuserdata(L, sizeof(func));
    memcpy(d, &func, sizeof(func));

    // C++11 lambda：显式指定返回值，避免转换错误
    lua_pushcclosure(L, [](lua_State* L) -> int {
        return wrapfunc<CALL_FUN>(L);
        }, 1);

    lua_rawset(L, -3);
}

// ===================== 6. 辅助函数（修复lambda转换问题，改用普通函数） =====================
namespace baseitem_detail {
    // func0：无参数无返回值
    static void func0_impl() {
        printf("无参数函数调用\n");
    }

    // func1：单int参数，返回int
    static int func1_impl(int a) {
        return a * 2;
    }

    // func2：多参数（int + string + bool），返回const char*
    static const char* func2_impl(int a, const char* s, bool b) {
        static char buf[128];
        snprintf(buf, sizeof(buf), "a=%d, s=%s, b=%s", a, s, b ? "true" : "false");
        return buf;
    }

    // func3：浮点数参数，返回double
    static double func3_impl(float a, double b) {
        return a + b;
    }
} // namespace baseitem_detail

// ===================== 7. 原有类定义 =====================
class baseitem
{
public:
    baseitem() {}
    ~baseitem() {}
public:
    void ToluaRegfuncsStart(lua_State* L);
    void ToluaRegfuncsEnd(lua_State* L);

protected:
    int getInt();

    virtual void ToluaRegfuncs(lua_State* L);
};
