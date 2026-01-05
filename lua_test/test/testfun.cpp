
#include <string>
#include <iostream>
#include <sstream>


#include "testfun.h"

using namespace std;


void t12345(int b)
{
	int a = 1 +b;
}



void baseitem::ToluaRegfuncsStart(lua_State* L)
{
	lua_newtable(L); //OriTable
	ToluaRegfuncs(L);
}


int baseitem::getInt()
{
	return 9;
}

void baseitem::ToluaRegfuncs(lua_State* L)
{
	lua_newtable(L); //meta table
	lua_newtable(L); //__index table

        // 1. 无参数无返回值
        void (*func0)() = []() {
            printf("无参数函数调用\n");
            };
        _ToluaRegfuncs(L, "func0", func0);

        // 2. 单int参数，返回int
        int (*func1)(int) = [](int a) {
            return a * 2;
            };
        _ToluaRegfuncs(L, "func1", func1);

        // 3. 多参数（int + string + bool），返回string
        const char* (*func2)(int, const char*, bool) = [](int a, const char* s, bool b)->const char* {
            static char buf[128];
            snprintf(buf, sizeof(buf), "a=%d, s=%s, b=%s", a, s, b ? "true" : "false");
            return buf;
            };
        _ToluaRegfuncs(L, "func2", func2);

        // 4. 浮点数参数，返回浮点数
        double (*func3)(float, double) = [](float a, double b) {
            return a + b;
            };
        _ToluaRegfuncs(L, "func3", func3);


	_ToluaRegfuncs(L,"tttt",&t12345);
	//_ToluaRegfuncs(L,"gggg",&baseitem::getInt); error todo
}

void baseitem::ToluaRegfuncsEnd(lua_State* L)
{
	lua_setfield(L,-2,"__index");
	lua_setmetatable(L,-2);
}


baseitem* _itm = nullptr;
int testGetInfo(lua_State* L)
{
	if (!_itm)
	{
		_itm = new baseitem(); //todo free
	}

	_itm->ToluaRegfuncsStart(L);
	_itm->ToluaRegfuncsEnd(L);

	if (!lua_istable(L,-1))
	{
		//assert(true);
	}

	return 1;
}

extern const string& Gdir();
extern int PcallErrorFunc(lua_State* L);

void addGfunc(lua_State* L)
{
	lua_getglobal(L,"_G");

	lua_pushstring(L,"testGetInfo");
	lua_pushcfunction(L,testGetInfo);
	lua_rawset(L, -3);
	
	lua_pop(L,1);
}

void test_matetable()
{
    lua_State* L= lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< 加载相关库文件
	luaL_openlibs(L);
    
	///< 加载lua文件
    std::string sf = Gdir()+ "/testfun.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		cout << lua_tostring(L, -1) << endl;
        return;
    }

	addGfunc(L);

	cout << "Lua 文件加载["<< sf <<"] 成功" << endl;

	//执行脚本加载后,默认的闭包方法,初始化当前脚本环境
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "脚本初始化失败" << endl;
        cout << lua_tostring(L, -1) << endl;
        return ;
    }


	lua_pushcfunction(L,PcallErrorFunc);
	int errfunc = lua_gettop(L);
	
	///指定 lua 入口函数 run 
    lua_getglobal(L, "run_main");
    //执行
	//if (lua_pcall(L, 0, 0, 0))
	if (lua_pcall(L, 0, 0, errfunc)) //设置了 errfunc 和没有设置,触发失败后的栈会不一样.设置了 errfunc 时,栈项的错误结果要在 errfunc 里处理
    {
		cout << "调用脚本入口方法失败" << endl;
		//cout << lua_tostring(L,-1) << endl;  //没有设置 errfunc 时,栈项的错误结果要在这里处理
        return ;
    }

}

