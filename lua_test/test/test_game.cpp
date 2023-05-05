#include <iostream>

extern "C" { // lua.dll����c�⣬ʹ��ʱҪ�� extern "C",  ��� lua�ⰴ c++������ľ���  __declspec(dllimport)/dllexport ����

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
	//			// ���ļ������ڴ�
	//			char * pData = new char[nLength + 1]; pData[nLength] = 0;
	//			pStream->read(pData, nLength);
	//			//pStream->close();
	//			// ͨ���ڴ�����ļ�
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
		lua_getfield(L, -1, "loaders");//�߰汾��lua���� "searchers"
		if (lua_istable(L, -1))
 		{
			lua_pushcfunction(L, fn);
			lua_rawseti(L, -2, 2); //�滻�ڶ�������. Ҳ���Զ����һ����
/*
lua�ڳ�ʼ��package.searchers���ʱ�򣬻��������4�����Һ�����
	��һ�����Һ���ֻ��package.preload���в���loader
	�ڶ������Һ�������package.path�е�����·��������loader��ƥ�������package.searchpath�е�����˵����ƥ��
	�������Ǽ���C���Կ�ģ���path��ͬ��
	���ĸ�Ҳ����������C���Կ�ģ��Ͳ�˵�ˣ�����Ҫ�Ĳ鿴����Ĺٷ��ĵ�
*/


			return 0;
		}
	}
	return -1;
}


int testGameLua(lua_State *L)
{


    ///< ����lua�ļ�
    std::string sf = game_scripts + "/GameMain.lua";
	//std::string sf = dir_scripts + "/test001.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
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

	cout << "Lua �ļ�����["<< sf <<"] �ɹ�" << endl;

	//ִ�нű����غ�,Ĭ�ϵıհ�����,��ʼ����ǰ�ű�����
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "�ű���ʼ��ʧ��" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }



    return 0;
}



