#include <iostream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;



static string dir_scripts = "../../test/scripts";


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
	
	//告诉lua主程序，返回1个值, 在脚本里可使用
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





//lua　使用 c++ 里的方法
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

//遍历 Table
bool DumpTable(lua_State* L, int idx)
{
    try{
        lua_pushnil(L);
/*
lua_next() 这个函数的工作过程是：
1) 先从栈顶弹出一个 key, 所以在使用 lua_next 之前会先压入一个 nil. lua_pushnil(L);
2) 从栈指定位置的 table 里取下一对 key-value，先将 key 入栈再将 value 入栈
3) 如果第 2 步成功则返回非 0 值，否则返回 0，并且不向栈中压入任何值
*/
        while(lua_next(L, idx) != 0){
            int keyType = lua_type(L, -2);
            if(keyType == LUA_TNUMBER){
                double value = lua_tonumber(L, -2);
                cout << "Key:" << value << endl;
            }else if(keyType == LUA_TSTRING){
                const char*  value = lua_tostring(L, -2);
                cout << "Key:" << value << endl;
            }else{
                cout << "Invalid key type: " << keyType << endl;
                return false;
            }
            int valueType = lua_type(L, -1);
            switch(valueType){
                case LUA_TNIL:
                {
                    cout << "Value: nil" << endl;
                    break;
                }
                case LUA_TBOOLEAN:
                {
                    int value = lua_toboolean(L, -1);
                    cout << value << endl;
                    break;
                }
                case LUA_TNUMBER:
                {    cout << "Value:" << lua_tonumber(L, -1) << endl;
                    break;
                }
                case LUA_TSTRING:
                {
                    cout << "Value:" << lua_tostring(L, -1) << endl;
                    break;
                }
                case LUA_TTABLE:
                {
 
                    cout << "====sub table===" << endl;
                    int index = lua_gettop(L);
                    if (!DumpTable(L, index)) {
                        cout << "popTable error in  popTable,error occured" << endl;
                        return false;
                    }
                    break;
                }
                default:
                {
                    cout << "Invalid value type: " << valueType << endl;
                    return false;
                }
            }
            lua_pop(L, 1);
        }
    }catch(const char* s){
       string errMsg = s;
       lua_pop(L,1);
       cout << errMsg << endl;
       return false;
    }catch(std::exception& e){
        const char* errMsg = e.what();
        lua_pop(L,1);
        cout << errMsg << endl;
        return false;
    }catch(...){
        const char* errMsg = lua_tostring(L,-1);
        lua_pop(L,1);
        cout << errMsg << endl;
        return false;
    }
    return true;
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
        return -2;
    }

    {

		//脚本加载后会解析成一个 Closure 入口方法,压入栈中.所以当前栈 top 的上一个就是这个实例,通过这个可以找到 Proto 
		p_func_proto(clvalue( pState->top - 1)->l.p);

		///< 执行lua文件 入口方法
		//LUA_API int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
		if (lua_pcall(pState, 0, 0, 0))
		{
			cerr << lua_tostring(pState, -1) << endl;
		}
		else
		{
			/// 获取 lua mystr 值
			lua_getglobal(pState, "mystr");
			string str = lua_tostring(pState, -1);
			string str2 = lua_tostring(pState, -1);
			cout << str << "  " << str2 << endl;
			lua_pop(pState, 1); 

			///< 获取 lua myTable 表中数据
			lua_getglobal(pState, "myTable");
			lua_getfield(pState, -1, "name");
			cout << lua_tostring(pState, -1) << endl;
			lua_pop(pState, 1); 


			lua_getfield(pState, -1, "id");
			cout << lua_tonumber(pState, -1) << endl;
			lua_pop(pState, 1); 


            int type = lua_type(pState, -1);
            if (type == LUA_TTABLE) {
                int index = lua_gettop(pState);
                if (DumpTable(pState, index)) {
                    return 0;
                }
                else {
                    cout << "Error" << endl;
                    return -1;
                }
            }


			//lua  myAry 数组
			lua_getglobal(pState, "myAry");
			auto n = lua_objlen(pState, -1);
			
			//打印数组
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);  //往栈里面压入i
				lua_gettable(pState, -2);  //table位于-2的位置, 读取table[i] 到 top。
				//lua_rawget(pState, -2);  //lua_gettable也可以用lua_rawget来替换
				cout << lua_tostring(pState, -1) << endl;
				lua_pop(pState, 1); //弹出top读取到的table[i]
			}

			//修改 数组
			for (int i = n+1; i <= n+2; ++i) {
				lua_pushnumber(pState, i);
				char buf[256];
				sprintf(buf, "hehe%d", i);
				lua_pushstring(pState, buf);
				//lua_settable(L, -3);
				lua_rawset(pState, -3);
			}

			//打印修改后数组
			n = lua_objlen(pState, -1);
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);  //往栈里面压入i
				lua_gettable(pState, -2);  //table位于-2的位置, 读取table[i] 到 top。
				//lua_rawget(pState, -2);  //lua_gettable也可以用lua_rawget来替换
				cout << lua_tostring(pState, -1) << endl;
				lua_pop(pState, 1); //弹出top读取到的table[i]
			}









			///< 调用 lua 函数 print_hello
			lua_getglobal(pState, "print_hello");
			lua_pcall(pState, 0, 0, 0);

			///< 调用 lua 函数 _add
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




/*
lua_newuserdata  函数按照指定的大小分配一块内存，将对应的 userdatum放到栈内,并返回内存块的地址。
*/
void TestUserdata(lua_State* L)
{

}


int ActFunc(lua_State* L)
{
	stackDump(L,0);
	
	//auto baseLuaState = G(L)->mainthread;
		
	lua_pushstring(L, "1");
	cout << "top:"<< L->top - L->base  << endl;
    lua_pushstring(L, "1");
	cout << "top:"<< L->top - L->base  << endl;
	
	stackDump(L,1);

	//ttisthread(baseLuaState->base);	
	//auto tp = lua_type(baseLuaState, 0);
	
	//stackDump(G(L)->mainthread);
	
	void* buffer;
	buffer = lua_touserdata(L, lua_upvalueindex(1)); //在 lua_pushcclosure 关联的数据

	cout <<"buffer store string:" << (char*)buffer <<endl;

	int a=0x123;
	


	return 0;
}
/*
int lua_pushcclosure(lua_State *L,lua_CFunction func,int n )
	其中n是堆栈顶端值得个数，这些值将复制到closure中，func是使用closure的函数，
	这个函数调用后会清除栈中n个值，会压入一个closure到栈中。
*/
void TestCClosure(lua_State* L)
{
	stackDump(L,1);
    
	lua_pushstring(L, "1");

	stackDump(L,1);
	stackDump(L,0);


    lua_pushstring(L, "1");
    lua_pushstring(L, "1");
    lua_pushstring(L, "1");

	cout << "top:"<< L->top - L->base  << endl;
    lua_CFunction function = &ActFunc;

	const char* str = "123";
    int sizeofstring= 32;
    lua_pushstring(L, "funcName");
	cout << "top:"<< L->top - L->base  << endl;
	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeofstring);
	cout << "top:"<< L->top - L->base  << endl;

	memset(buffer, '\0',strlen(str) ); 
	memcpy(buffer, str,strlen(str) ); //使用 buffer,保存 str,未带结束符
	lua_pushcclosure(L, function, 1);
		/*
			这里 n=1,有一个关联参数,会把上边那个 lua_newuserdata 复制到 closuer,之后在栈里弹出.
			所以后面打印栈顶时,数量会差一个 
		*/
	cout << "top:"<< L->top - L->base  << endl;

	stackDump(L,1);
	
	
	lua_pcall(L, 0, 0, 0); //执行 cclosure
	
	stackDump(L,1);

}





void* GetObjectIndexData1(lua_State* L)
{
    lua_pushstring(L, "1");

    cout << "top:" << L->top - L->base << endl;

    lua_pushstring(L, "2");

    cout << "top:" << L->top - L->base << endl;
    lua_pushstring(L, "3");

    cout << "top:" << L->top - L->base << endl;

    auto sz = sizeof(TValue);
    StkId o = restorestack(L, sz);
    /*
使用 restorestack 这个宏来定位栈上的地址,是因为数据栈的内存地址是会随着数据栈的大小而变化。
保存地址是不可能的，而应该记住一个相对量。
 savestack 和 restorestack 这两个宏就是做这个工作的。

#define savestack(L,p) ((char )(p) - (char )L->stack)
#define restorestack(L,n) ((TValue )((char )L->stack + (n)))

        */

    stackDump(L,0);

    return NULL;
}


//lua_yield, resume
void threadTest(lua_State* L)
{
    lua_getglobal(L, "Func1");
    lua_pushinteger(L, 10); //要传入的参数

    // 运行这个协同程序, 传入一个参数
    // 这里返回 bRet 对应当前 L->status : 0,OK/ 1,LUA_YIELD / 2,LUA_ERRRUN	

    auto bRet = lua_resume(L, 1); // 脚本方法 Func1 里会调用到 coroutine.yield
    cout << " L->status bRet:" << bRet << endl;

	// 脚本  coroutine.yield 传入的参数会压到栈上,最后会把参数个数压入
    // 打印L1栈中参数的个数
    cout << "Element Num:" << lua_gettop(L) << endl;

    // 当前 yield 返回的两个值
    cout << "Value 1:" << lua_tointeger(L, -2) << endl;
    cout << "Value 2:" << lua_tointeger(L, -1) << endl;

    // 再次启动协同程序, 传入1个参数
    // 这里返回0
    lua_pushinteger(L, 123); //要传入的参数, 在脚本里通过 coroutine.yield 返回值接收 
    bRet = lua_resume(L, 1); //再次调度到 Lua线程 之前 yield 处 
    cout << "bRet:" << bRet << endl;
    cout << "Element Num:" << lua_gettop(L) << endl;
    cout << "Value 1:" << lua_tointeger(L, -1) << endl;
}








void testMetatable(lua_State* L)
{
    // 获取 lua myTable 
    //lua_getglobal(L, "myTable");
}

/*
void CLuaBaseMetaClass::RegisterMetaClass(lua_State * pLuaState) const
{
	luaL_newmetatable(pLuaState, GetMetaClassName());
	lua_pushstring(pLuaState, "__gc");
	lua_pushcclosure(pLuaState, &CLuaBaseMetaClass::DoGarbageCollect, 0);
	lua_settable(pLuaState, -3);
	lua_pushstring(pLuaState, "__index");
	lua_newtable(pLuaState);
	RegisterMemberFunctions(pLuaState);
	lua_settable(pLuaState, -3);
	lua_pop(pLuaState, 1);
}
*/


int luaTestFunc(lua_State *L)
{
	int n = lua_gettop(L); //当前栈压了几个数据(参数)

	lua_getmetatable(L,1);
	
	return 0; //0个返回值
}


//typedef char *C,*CCC;

extern void testMapPath();
extern void testAVL();
int main(int argc, char* argv[])
{
	//test001(); // 有打印 Proto
	//test002();
	//test003();
    
	//testMapPath();
	testAVL();
	return 0;



    lua_State* L= lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< 加载相关库文件
	luaL_openlibs(L);

	///< 加载lua文件
	std::string sf = dir_scripts + "/test004.lua";
	//std::string sf = dir_scripts + "/test001.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }

	cout << "Lua 文件加载["<< sf <<"] 成功" << endl;

    //p_func_proto(clvalue(L->top - 1)->l.p); //dump 默认的闭包方法
    
	//执行脚本加载后,默认的闭包方法,初始化当前脚本环境
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "脚本初始化失败" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	lua_register(L, "TestFunc", luaTestFunc);
	
	//测试方法
    //TestCClosure(L);
    //GetObjectIndexData1(L);
    //testMetatable(L);
    threadTest(L);

	///指定 lua 入口函数 run 
    lua_getglobal(L, "run_main");
    //执行
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "调用脚本入口方法失败" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	system("pause");
	return 0;
}



