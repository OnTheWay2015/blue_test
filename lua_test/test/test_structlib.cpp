
#include <windows.h>


#include <iostream>
#include <sstream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;


namespace TEST_STRUCT {

static string dir_scripts = "../../../test/scripts";


int getGUID(lua_State* L)
{
	GUID gid{ 'a','b','c' };
	lua_pushlstring(L,(char*)(&gid),sizeof(GUID));

	return 1;
}


int testGUID(lua_State* L)
{
	size_t len = 0;
	const char* buf = lua_tolstring(L, 1, &len);

	GUID* p = (GUID*)(buf);
	return 0;
}


void addGfunc(lua_State* L)
{
	lua_getglobal(L,"_G");

	lua_pushstring(L,"getGUID");
	lua_pushcfunction(L,getGUID);
	lua_rawset(L, -3);

	lua_pushstring(L,"testGUID");
	lua_pushcfunction(L,testGUID);
	lua_rawset(L, -3);



	lua_pop(L,1);
}


int test()
{
    lua_State* L= lua_open();
	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< 加载相关库文件
	luaL_openlibs(L);
    

    ///< 加载lua文件
    std::string sf = dir_scripts + "/test_struct.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }
	cout << "Lua 文件加载["<< sf <<"] 成功" << endl;
    
	//执行脚本加载后,默认的闭包方法,初始化当前脚本环境
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "脚本初始化失败" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }



	addGfunc(L);
	

	//指定 lua 入口函数 run 
    lua_getglobal(L, "run_main");
    //执行
	if (lua_pcall(L, 0, 0, 0))
	//if (lua_pcall(L, 0, 0, errfunc)) //设置了 errfunc 和没有设置,触发失败后的栈会不一样.设置了 errfunc 时,栈项的错误结果要在 errfunc 里处理
    {
		cout << "调用脚本入口方法失败" << endl;
		cout << lua_tostring(L,-1) << endl;  //没有设置 errfunc 时,栈项的错误结果要在这里处理
        return -3;
    }

    lua_close(L);

    while (true)
    {
        Sleep(100);
    }
	system("pause");
	return 0;
}









void testStructLib()
{
    GUID gid{'a','b','c'};
}


}
