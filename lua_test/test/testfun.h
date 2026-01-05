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

// 确保仅使用C++11特性
#if __cplusplus < 201103L
#error "This code requires C++11 or later"
#endif

extern "C" {
#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"
}

// ===================== 关键修复：添加命名空间避免冲突 =====================
//namespace lua_bind_detail { // 自定义命名空间，隔离手动实现的模板
//    // 基础索引序列模板
//    template <std::size_t... Ints>
//    struct index_sequence {};
//
//    // 索引序列生成器（递归）
//    template <std::size_t N, std::size_t... Ints>
//    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ints...> {};
//
//    // 终止条件  报错，这里没有定义 type
//    template <std::size_t... Ints>
//    struct make_index_sequence<0, Ints...> : index_sequence<Ints...> {};
//} // namespace lua_bind_detail

namespace lua_bind_detail { // 自定义命名空间，隔离手动实现的模板
    // 基础索引序列模板
    template <std::size_t... Ints>
    struct index_sequence {};

    // 索引序列生成器（递归）
    template <std::size_t N, std::size_t... Ints>
    struct make_index_sequence : make_index_sequence<N - 1, N - 1, Ints...> {
        // 修复：添加type嵌套类型（继承父类的type）
        typedef typename make_index_sequence<N - 1, N - 1, Ints...>::type type;
    };

    // 终止条件：特化版本，定义最终的type
    template <std::size_t... Ints>
    struct make_index_sequence<0, Ints...> : index_sequence<Ints...> {
        // 核心修复：定义type为当前的index_sequence类型
        typedef index_sequence<Ints...> type;
    };
} // namespace lua_bind_detail


// ===================== 2. 类型转换体系（无修改） =====================
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

// 读取单个参数（带类型检查，C++11）
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

// ===================== 3. C++11 递归模板读取参数包（无修改） =====================
// 终止条件：空参数包
template<typename... Args>
struct ArgsReader;

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
        // 读取当前参数
        T arg = read_arg<T>(L, idx);
        // 递归读取剩余参数
        auto rest_args = ArgsReader<Rest...>::read(L, idx + 1);
        // 拼接tuple（C++11 std::tuple_cat）
        return std::tuple_cat(std::make_tuple(arg), rest_args);
    }
};

// 对外接口：读取所有参数
template<typename... Args>
std::tuple<Args...> read_args(lua_State* L) {
    // 检查参数数量
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
    // 从索引1开始读取
    return ArgsReader<Args...>::read(L, 1);
}

// ===================== 4. C++11 函数调用：参数包展开（修改命名空间引用） =====================
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

// 推送返回值（C++11 模板特化替代if constexpr）
template<typename T>
struct ReturnPusher;

// int返回值
template<>
struct ReturnPusher<int> {
    static void push(lua_State* L, int value) {
        lua_pushinteger(L, value);
    }
};

// float返回值
template<>
struct ReturnPusher<float> {
    static void push(lua_State* L, float value) {
        lua_pushnumber(L, value);
    }
};

// double返回值
template<>
struct ReturnPusher<double> {
    static void push(lua_State* L, double value) {
        lua_pushnumber(L, value);
    }
};

// const char*返回值
template<>
struct ReturnPusher<const char*> {
    static void push(lua_State* L, const char* value) {
        lua_pushstring(L, value);
    }
};

// bool返回值
template<>
struct ReturnPusher<bool> {
    static void push(lua_State* L, bool value) {
        lua_pushboolean(L, value ? 1 : 0);
    }
};

// void返回值（空实现）
template<>
struct ReturnPusher<void> {
    static void push(lua_State* /*L*/) {}
};

// 通用函数包装器（C++11）
template<typename Func>
struct FunctionWrapper;

// 特化：函数指针（返回值+参数包）
template<typename Ret, typename... Args>
struct FunctionWrapper<Ret(*)(Args...)> {
    static int call(lua_State* L, Ret(*func)(Args...)) {
        // 1. 读取参数
        auto args = read_args<Args...>(L);

        // 2. 生成索引序列（引用自定义命名空间）
        typedef typename lua_bind_detail::make_index_sequence<sizeof...(Args)>::type Indices;

        // 3. 调用函数并处理返回值
        if (std::is_void<Ret>::value) {
            // 无返回值
            call_func_void(func, args, Indices());
            return 0;
        }
        else {
            // 有返回值
            Ret result = call_func_ret<Ret>(func, args, Indices());
            ReturnPusher<Ret>::push(L, result);
            return 1;
        }
    }
};

// ===================== 5. 原有逻辑适配（C++11，无修改） =====================
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

    // C++11 lambda不能捕获模板参数，直接用wrapfunc模板
    lua_pushcclosure(L, [](lua_State* L) -> int {
        // 此处需要确保CALL_FUN类型正确，C++11需显式指定
        return wrapfunc<CALL_FUN>(L);
        }, 1);

    lua_rawset(L, -3);
}

// ===================== 6. 原有类定义（无修改） =====================
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

    virtual void ToluaRegfuncs(lua_State* L) ;
    //virtual void ToluaRegfuncs(lua_State* L) 
    //{
    //    // 1. 无参数无返回值
    //    void (*func0)() = []() {
    //        printf("无参数函数调用\n");
    //        };
    //    _ToluaRegfuncs(L, "func0", func0);

    //    // 2. 单int参数，返回int
    //    int (*func1)(int) = [](int a) {
    //        return a * 2;
    //        };
    //    _ToluaRegfuncs(L, "func1", func1);

    //    // 3. 多参数（int + string + bool），返回string
    //    const char* (*func2)(int, const char*, bool) = [](int a, const char* s, bool b) {
    //        static char buf[128];
    //        snprintf(buf, sizeof(buf), "a=%d, s=%s, b=%s", a, s, b ? "true" : "false");
    //        return buf;
    //        };
    //    _ToluaRegfuncs(L, "func2", func2);

    //    // 4. 浮点数参数，返回浮点数
    //    double (*func3)(float, double) = [](float a, double b) {
    //        return a + b;
    //        };
    //    _ToluaRegfuncs(L, "func3", func3);
    //}
};




//======================
