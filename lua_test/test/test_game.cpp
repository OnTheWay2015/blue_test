#include <iostream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;




static string game_scripts = "../../../test/scripts/lua";



static int TestLoadFile(lua_State *L, const char *name)
{
	//int status = -1;
	//char filename[256];
	//sprintf(filename, "%s.lua", name);
	//FileSystem * pFileSystem = getFileSystem();
	//if(pFileSystem != NULL)
	//{
	//	Stream * pStream = pFileSystem->open(filename);
	//	if(pStream != NULL)
	//	{
	//		Uint32 nLength = pStream->getLength();
	//		if(nLength > 0)
	//		{
	//			// 把文件读进内存
	//			char * pData = new char[nLength + 1]; pData[nLength] = 0;
	//			pStream->read(pData, nLength);
	//			//pStream->close();
	//			// 通过内存加载文件
	//			status = luaL_loadbuffer(L, pData, nLength, pData);
	//			delete[] pData;
	//		}
	//		pStream->release();
	//	}
	//}
	//return status;

	return 0;
}


int TestSetLoader(lua_State *L, lua_CFunction fn)
{
	lua_getglobal(L, LUA_LOADLIBNAME);
	if (lua_istable(L, -1)) 
	{
		lua_getfield(L, -1, "loaders");//高版本的lua是用 "searchers"
		if (lua_istable(L, -1))
 		{
			lua_pushcfunction(L, fn);
			lua_rawseti(L, -2, 2); //替换第二个方法. 也可以多添加一个？
/*
lua在初始化package.searchers表的时候，会添加如下4个查找函数：
	第一个查找函数只在package.preload表中查找loader
	第二个查找函数，按package.path中的配置路径来查找loader，匹配过程用package.searchpath中的配置说明来匹配
	第三个是加载C语言库的，跟path的同样
	第四个也是用来加载C语言库的，就不说了，有须要的查看这里的官方文档
*/


			return 0;
		}
	}
	return -1;
}


int testGameLua(lua_State *L)
{


    ///< 加载lua文件
    std::string sf = game_scripts + "/GameMain.lua";
	//std::string sf = dir_scripts + "/test001.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }

    /*
			int Ret = luaL_loadbuffer(GetLuaState(), szScriptContent, ContentLen, szScriptName);
			if (Ret == 0)
			{
				Ret = lua_pcall(GetLuaState(), 0, LUA_MULTRET, 0);
			}

	*/

	cout << "Lua 文件加载["<< sf <<"] 成功" << endl;

	//执行脚本加载后,默认的闭包方法,初始化当前脚本环境
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "脚本初始化失败" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }



    return 0;
}



