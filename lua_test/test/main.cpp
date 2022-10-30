#include <iostream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"

}

using namespace std;


static string dir_scripts = "D:/work/wh_svr/lua_test/test/scripts";


int n_act01(lua_State* L)
{
	//lua_gettop 是取出栈顶的索引值。此时栈顶的索引值大小就是栈内元素的个数
	int n = lua_gettop(L);
	double sum = 0;
	//使用循环变量站内所有的元素，通过lua_tonumber取出栈内的值，然后进行相加操作。
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//将运算后的值返还给Lua。把要返回的值再压入栈。此时此时栈内7条数据，参考栈的运行图Log index 2
	//lua_pushnumber(L, sum / n);//average
	lua_pushnumber(L, sum);//sum
	
	//告诉lua主程序，返回2个值。lua这是可以用参数接受这两个值
	return 1;
}


int n_act02(lua_State* L)
{
	//lua_gettop 是取出栈顶的索引值。此时栈顶的索引值大小就是栈内元素的个数
	int n = lua_gettop(L);
	double sum = 0;
	//使用循环变量站内所有的元素，通过lua_tonumber取出栈内的值，然后进行相加操作。
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//将运算后的值返还给Lua。把要返回的值再压入栈。此时此时栈内7条数据，参考栈的运行图Log index 2
	lua_pushnumber(L, sum / n);//average
	//lua_pushnumber(L, sum);//sum
	
	//告诉lua主程序，返回2个值。lua这是可以用参数接受这两个值
	return 1;
}

static const luaL_Reg testlib[] = {
  {"act01", n_act01},
  {"act02", n_act02},
  {NULL, NULL}
};



/*
lua


*/
//lua　使用 c++ 里注册的模块
void test003()
{
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< 加载相关库文件
	luaL_openlibs(pState);

	luaL_register(pState, "testlib", testlib);
	
	///< 加载lua文件
	std::string sf = dir_scripts + "/test003.lua";
	if (luaL_loadfile(pState, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		return;
	}
	
	if (lua_pcall(pState, 0, 0, 0))
	 {
		 cout << lua_tostring(pState, -1) << endl;
		 return;
	 }
	 
	 
	 lua_close(pState);
}











/*
lua

avg, sum = average(10,20,30,40,50);
print("The average is ", avg)
print("The sum is ", sum)


*/
int Average(lua_State *L)
{
	//code3   lua_gettop 是取出栈顶的索引值。此时栈顶的索引值大小就是栈内元素的个数
	int n = lua_gettop(L);
	double sum = 0;
	//code4 使用循环变量站内所有的元素，通过lua_tonumber取出栈内的值，然后进行相加操作。
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//code5 将运算后的值返还给Lua。把要返回的值再压入栈。此时此时栈内7条数据，参考栈的运行图Log index 2
	lua_pushnumber(L, sum / n);//average
	lua_pushnumber(L, sum);//sum
	//code6 告诉lua主程序，返回2个值。lua这是可以用参数接受这两个值
	return 2;

}





//lua　使用 c++ 里的数据
void test002()
{
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< 加载相关库文件
	luaL_openlibs(pState);

	///< 加载lua文件
	std::string sf = dir_scripts + "/test002.lua";
	if (luaL_loadfile(pState, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		return;
	}

	//code1 lua_register 注册函数把 Lua 函数和 C++ 函数进行绑定。
	//其实就是先用lua_pushcfunction把在c++中定义的函数压如栈中，
	//然后lua_setglobal来设置栈顶的元素对应的值，这样就可以把lua函数和栈顶的c++函数建立引用关系。
	lua_register(pState, "average", Average);
	//code2  加载并执行lua脚本，此时lua中的函数average被执行，同时向栈中压如5个参数。参考栈的运行图Log index 1
	//luaL_dofile(L, "testLC.lua");
	 if (lua_pcall(pState, 0, 0, 0))
	 {
		 cout << lua_tostring(pState, -1) << endl;
		 return;
	 }
	 
	 
	 lua_close(pState);
}


//c++ 使用 lua　里的数据
int test001()
{
	///< 创建lua句柄并初始化
	//lua_State *pState = luaL_newstate();
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< 加载相关库文件
	luaL_openlibs(pState);

	///< 加载lua文件
	std::string sf = dir_scripts + "/test001.lua";
	if (luaL_loadfile(pState, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
	}
	else
	{
		///< 执行lua文件
		if (lua_pcall(pState, 0, 0, 0))
		{
			cerr << lua_tostring(pState, -1) << endl;
		}
		else
		{
			///< 获取值
			lua_getglobal(pState, "mystr");

			string str = lua_tostring(pState, -1);
			cout << str << endl;

			///< 获取表中数据
			lua_getglobal(pState, "myTable");
			lua_getfield(pState, -1, "name");
			cout << lua_tostring(pState, -1) << endl;

			lua_getglobal(pState, "myTable");
			lua_getfield(pState, -1, "id");
			cout << lua_tonumber(pState, -1) << endl;

			//数组
			lua_getglobal(pState, "myAry");
			auto n = lua_objlen(pState, -1);
			//修改 数组
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);
				char buf[256];
				sprintf(buf, "hehe%d", i);
				lua_pushstring(pState, buf);
				//lua_settable(L, -3);
				lua_rawset(pState, -3);
			}

			//打印数组
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);  //往栈里面压入i
				lua_gettable(pState, -2);  //读取table[i]，table位于-2的位置。
				//lua_rawget(pState, -2);  //lua_gettable也可以用lua_rawget来替换
				cout << lua_tostring(pState, -1) << endl;
				lua_pop(pState, 1);
			}









			///< 调用函数
			lua_getglobal(pState, "print_hello");
			lua_pcall(pState, 0, 0, 0);

			///< 调用计算函数
			lua_getglobal(pState, "_add");
			lua_pushnumber(pState, 10);
			lua_pushnumber(pState, 20);

			if (lua_pcall(pState, 2, 1, 0))
			{
				cout << lua_tostring(pState, -1) << endl;
				lua_close(pState);
			}
			else
			{
				if (lua_isnumber(pState, -1))
				{
					cout << lua_tonumber(pState, -1) << endl;
				}
			}
		}
	}

	lua_close(pState);


	return 0;
}









int main(int argc, char* argv[])
{
	test001();
	//test002();
	//test003();
	system("pause");
	return 0;
}
