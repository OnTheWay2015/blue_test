#include <iostream>
#include <sstream>

extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;



static string dir_scripts = "../../../test/scripts";
bool DumpTable(lua_State* L, int idx);

int LuaPrintIdx(lua_State* L, int idx);
int LuaPrint(lua_State* L);

const string& Gdir()
{
	return dir_scripts;
}

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
void  lua_insert(lua_State *L, int idx)
移指定位置上的所有元素以开辟一个空间槽的空间，然后将栈顶元素移到该位置

for (int i = 1; i <= 5; ++i)
      lua_pushnumber(i);

栈中元素：（从下往上） 1 2 3 4 5

lua_insert(L, 3)

栈中元素：（从下往上） 1 2 5 4 3

*/





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

int LuaTestArgs(lua_State *L)
{
	//lua_gettop 是取出栈顶的索引值。此时栈顶的索引值大小就是栈内元素的个数
	int n = lua_gettop(L);
	double sum = 0;
	
	//从1开始,访问栈内元素，通过对应方法 lua_tonumber/ ..., 取出栈内的值，然后进行相加操作。
	{
		int idx = 1;
		auto v = lua_tonumber(L, idx);
		cout << "argn["<<n<<"]  idx["<<idx<<"] v["<<v<<"]" << endl;

	}
	{
		int idx = 2;
		auto v = lua_tointeger(L, idx);
		cout << "argn["<<n<<"]  idx["<<idx<<"] v["<<v<<"]" << endl;

	}
	{
		int idx = 3;
		auto v = lua_toboolean(L, idx);
		cout << "argn["<<n<<"]  idx["<<idx<<"] v["<<v<<"]" << endl;

	}
	
	{
		int idx = 4;
		auto v = lua_tolstring(L, idx,nullptr);
		cout << "argn["<<n<<"]  idx["<<idx<<"] v["<<v<<"]" << endl;

	}
	{
		int idx = 5;
		auto v = lua_topointer(L, idx);
		cout << "argn["<<n<<"]  idx["<<idx<<"] v["<<v<<"]" << endl;

	}
    {
        //array
        int idx = 6;
        cout << "argn[" << n << "]  idx[" << idx << "] array: " << endl;
        DumpTable(L, idx);

        auto len = lua_objlen(L, idx);

        //打印数组
        for (int i = 1; i <= len; ++i) {
            lua_pushnumber(L, i);  //往栈里面压入i
            lua_gettable(L, idx);  //table位于 idx 的位置, 读取table[i] 到 top。
            //lua_rawget(L, idx);  //lua_gettable也可以用lua_rawget来替换
            cout << lua_tostring(L, -1) << endl;
            lua_pop(L, 1); //弹出top读取到的table[i]
        }

        //修改 数组
        for (int i = 1; i <= len; ++i) {
            lua_pushnumber(L, i);
            char buf[256];
            sprintf(buf, "hehe%d", i);
            lua_pushstring(L, buf);
            //lua_settable(L, -3);// table 位于 -3 的位置; -1,是要修改值在 table 的下标;-2 是新值
            lua_rawset(L, idx);
        }

        //打印修改后数组
        for (int i = 1; i <= len; ++i) {
            lua_pushnumber(L, i);  //往栈里面压入i
            lua_gettable(L, idx);  //table位于-2的位置, 读取table[i] 到 top。
            //lua_rawget(L, -2);  //lua_gettable也可以用lua_rawget来替换
            cout << lua_tostring(L, -1) << endl;
            lua_pop(L, 1); //弹出top读取到的table[i]
        }


    }
	{
//table
		int idx = 7;
		cout << "argn["<<n<<"]  idx["<<idx<<"] table: " << endl;
		DumpTable(L,idx);
	
		lua_getfield(L, idx, "x"); //table位于 idx 的位置, 结果压栈
        cout << "x:" << lua_tonumber(L, -1) << endl;
        lua_pop(L, 1);
	}



//bool DumpTable(lua_State* L, int idx)

/*
LUA_API size_t          (lua_objlen) (lua_State *L, int idx);
LUA_API lua_CFunction   (lua_tocfunction) (lua_State *L, int idx);
LUA_API void	       *(lua_touserdata) (lua_State *L, int idx);
LUA_API lua_State      *(lua_tothread) (lua_State *L, int idx);
*/	


	
	lua_pushnumber(L, 1);//res 把要返回的值压入栈
	return 1; //告诉lua主程序，返回1个值。lua这是可以用参数接受这两个值

}

int LuaDumpObj(lua_State *L)
{
	DumpTable(L,1);//打印第一个参数
	return 1;
}


//lua　使用 c++ 里的方法
void test002()
{
	lua_State *L = lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< 加载相关库文件
	luaL_openlibs(L);

	///< 加载lua文件
	std::string sf = dir_scripts + "/test002.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua 文件加载失败" << endl;
		return;
	}

	//code1 lua_register 注册函数把 Lua 函数和 C++ 函数进行绑定。
	//其实就是先用lua_pushcfunction把在c++中定义的函数压如栈中，
	//然后lua_setglobal来设置栈顶的元素对应的值，这样就可以把lua函数和栈顶的c++函数建立引用关系。
	lua_register(L, "average", Average);
	lua_register(L, "testArgs", LuaTestArgs);
	lua_register(L, "DUMP", LuaDumpObj);
	//code2  加载并执行lua脚本，此时lua中的函数average被执行，同时向栈中压如5个参数。参考栈的运行图Log index 1
	//luaL_dofile(L, "testLC.lua");
	 if (lua_pcall(L, 0, 0, 0))
	 {
		 cout << lua_tostring(L, -1) << endl;
		 return;
	 }
	



    lua_getglobal(L, "run_main");
	int nargs = 6; //压入参数个数
	lua_pushnumber(L,123);
	lua_pushinteger(L,999);
	lua_pushstring(L,"string");
	lua_pushboolean(L,false);

	lua_newtable(L);//array 压栈 
	for (auto ii=0;ii<5;ii++)
    {
        lua_pushnumber(L, ii+1);  // push key 
        lua_pushnumber(L, ii);  //push value
        lua_settable(L, -3);  // table["key1"] = 199, pop key(-2) & value(-1)
    }


	
	lua_newtable(L);//table 压栈
	lua_pushstring(L,"key1");  // push key 
	lua_pushnumber(L,199);  //push value
    lua_settable(L,-3);  // table["key1"] = 199, pop key(-2) & value(-1)

	lua_pushstring(L,"key2");  // push key 
	lua_pushnumber(L,300);  //push value
    lua_settable(L,-3);  // table["key2"] = 300, pop key(-2) & value(-1)


    //执行
	if (lua_pcall(L, nargs, 0, 0)) //设置了 errfunc 和没有设置,触发失败后的栈会不一样.设置了 errfunc 时,栈项的错误结果要在 errfunc 里处理
    {
		cout << "调用脚本入口方法失败" << endl;
		cout << lua_tostring(L,-1) << endl;  //没有设置 errfunc 时,栈项的错误结果要在这里处理
        return ;
    }


 
	 
	 lua_close(L);
}

//遍历 Table
bool DumpTable(lua_State* L, int idx)
{
    //try{
        lua_pushnil(L);
/*
lua_next() 这个函数的工作过程是：
1) 先从栈顶弹出一个 key, 所以在使用 lua_next 之前会先压入一个 nil. lua_pushnil(L);
2) 从栈指定位置的 table 里取下一对 key-value，先将 key 入栈再将 value 入栈
3) 如果第 2 步成功则返回非 0 值，否则返回 0，并且不向栈中压入任何值
*/
        while(lua_next(L, idx) != 0){// 1), 弹一个 nil 出去后,压入一对 key_value
            int keyType = lua_type(L, -2); //key 在  -2， value 在 -1
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
                case LUA_TFUNCTION		:
                {
                    cout << "Value: function" << endl;
                    break;
                }
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
    //}
	//catch(const char* s){
    //   string errMsg = s;
    //   lua_pop(L,1);
    //   cout << errMsg << endl;
    //   return false;
    //}catch(std::exception& e){
    //    const char* errMsg = e.what();
    //    lua_pop(L,1);
    //    cout << errMsg << endl;
    //    return false;
    //}catch(...){
    //    const char* errMsg = lua_tostring(L,-1);
    //    lua_pop(L,1);
    //    cout << errMsg << endl;
    //    return false;
    //}
    return true;
}
 


//c++ 使用 lua　里的数据
int test001()
{
	///< 创建lua句柄并初始化
	//lua_State *pState = luaL_newstate();
	lua_State *L = lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< 加载相关库文件
	luaL_openlibs(L);

	///< 加载lua文件
	std::string sf = dir_scripts + "/test001.lua";
    if (luaL_loadfile(L, sf.c_str()))
    {
        cout << "Lua 文件加载失败" << endl;
        return -2;
    }

    {

		//脚本加载后会解析成一个 Closure 入口方法,压入栈中.所以当前栈 top 的上一个就是这个实例,通过这个可以找到 Proto 
		p_func_proto(clvalue( L->top - 1)->l.p);

		///< 执行lua文件 入口方法
		//LUA_API int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
		if (lua_pcall(L, 0, 0, 0))
		{
			cerr << lua_tostring(L, -1) << endl;
		}
		else
		{
			/// 获取 lua mystr 值
			lua_getglobal(L, "mystr");
			string str = lua_tostring(L, -1);
			string str2 = lua_tostring(L, -1);
			cout << str << "  " << str2 << endl;
			lua_pop(L, 1); 

			///< 获取 lua myTable 表中数据
			lua_getglobal(L, "myTable");
			lua_getfield(L, -1, "name");//table位于 -1 的位置, 结果压栈
			cout << lua_tostring(L, -1) << endl;
			lua_pop(L, 1); 


			lua_getfield(L, -1, "id");//table位于 -1 的位置, 结果压栈
			cout << lua_tonumber(L, -1) << endl;
			lua_pop(L, 1); 


            int type = lua_type(L, -1);//dump 打印 myTable
            if (type == LUA_TTABLE) {
                int index = lua_gettop(L);
                if (DumpTable(L, index)) {
                    return 0;
                }
                else {
                    cout << "Error" << endl;
                    return -1;
                }
            }

			//lua  myAry 数组
			lua_getglobal(L, "myAry");
			auto n = lua_objlen(L, -1);
			
			//打印数组
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(L, i);  //往栈里面压入i
				lua_gettable(L, -2);  //table位于-2的位置, 读取table[i] 到 top。
				//lua_rawget(L, -2);  //lua_gettable也可以用lua_rawget来替换
				cout << lua_tostring(L, -1) << endl;
				lua_pop(L, 1); //弹出top读取到的table[i]
			}

			//修改 数组
			for (int i = n+1; i <= n+2; ++i) {
				lua_pushnumber(L, i);
				char buf[256];
				sprintf(buf, "hehe%d", i);
				lua_pushstring(L, buf);
				//lua_settable(L, -3);// table 位于 -3 的位置; -1,是要修改值在 table 的下标;-2 是新值
				lua_rawset(L, -3);
			}

			//打印修改后数组
			n = lua_objlen(L, -1);
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(L, i);  //往栈里面压入i
				lua_gettable(L, -2);  //table位于-2的位置, 读取table[i] 到 top。
				//lua_rawget(L, -2);  //lua_gettable也可以用lua_rawget来替换
				cout << lua_tostring(L, -1) << endl;
				lua_pop(L, 1); //弹出top读取到的table[i]
			}









			///< 调用 lua 函数 print_hello
			lua_getglobal(L, "print_hello");
			lua_pcall(L, 0, 0, 0);

			///< 调用 lua 函数 _add
			lua_getglobal(L, "_add");
			lua_pushnumber(L, 10);
			lua_pushnumber(L, 20);

			if (lua_pcall(L, 2, 1, 0))
			{
				cout << lua_tostring(L, -1) << endl;
				lua_close(L);
			}
			else
			{
				if (lua_isnumber(L, -1))
				{
					cout << lua_tonumber(L, -1) << endl;
				}
			}
		}
	}

	lua_close(L);


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
	buffer = lua_touserdata(L, lua_upvalueindex(1)); //在 lua_pushcclosure 关联的数据,查看 TestCClosure-->lua_pushcclosure

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

void testtable(lua_State* L)
{
//查看栈 L->top - L->stack 
    lua_newtable(L);
    //lua_pushinteger(L, 123);
    lua_pushstring(L, "123");
    lua_pushstring(L, "aaaa");
    //lua_newtable(L);
    lua_settable(L, -3);

	LuaPrintIdx(L,-1);

    lua_newtable(L);
	lua_pushstring(L, "key");  
    lua_pushstring(L, "123");
    lua_settable(L, -3);



}


/*
debug.traceback()

lua_getglobal(tolua_S, "debug");  
lua_getfield(tolua_S, -1, "traceback");  
int iError = lua_pcall( tolua_S,//VMachine    
            0,//Argument Count    
            1,//Return Value Count    
            0);   
const char* sz = lua_tostring(tolua_S, -1); 

*/
int PcallErrorFunc(lua_State *L)
{
	//lua_pushvalue(L,lua_gettop(L));
	cout << lua_tostring(L,-1) << endl;
	return 0;
}
int debugTraceBack(lua_State *L) //todo test 
{
	lua_pushcfunction(L,PcallErrorFunc);
	int errfunc = lua_gettop(L);

    lua_getglobal(L, "debug");
    lua_getfield(L, -1, "traceback");
    if (lua_pcall(L, 0, 0, errfunc))
	{
	}
    return 0;
}



int panichandler(lua_State *L)
{
    printf("%s\n", lua_tostring(L, -1));
    return 0;
}

//static int panic (lua_State *L) {
//  (void)L;  /* to avoid warnings */
//  fprintf(stderr, "PANIC: unprotected error in call to Lua API (%s)\n",
//                   lua_tostring(L, -1));
//  return 0;
//}

void testHandler(lua_State *L)
{
		
	lua_atpanic(L, panichandler);//设置新panic函数并返回旧的panic函数
}


//typedef char *C,*CCC;


void DumpLuaValue(lua_State* L, int Index, std::stringstream& OutBugger, std::string PrevStr, bool ForceSameLine, int Deep)
{
	if ((!ForceSameLine))
		OutBugger << (("\r\n"));
	Deep++;
	if (Deep > 8)
	{
		OutBugger << (("reach max deep"));
		return;
	}	

	int Type = lua_type(L, Index);
	std::string Temp, Space;
	auto tabkey = std::string("    ");
	switch (Type)
	{
	case LUA_TNIL:
		//Temp.Format(("%snil"), PrevStr);
		OutBugger << PrevStr << "nil";
		break;
	case LUA_TBOOLEAN:
		if (lua_toboolean(L, Index))
			OutBugger << PrevStr << "true";
		else
			OutBugger << PrevStr << "false";
		break;
	case LUA_TLIGHTUSERDATA:
		OutBugger << PrevStr << "LightUserData";		
	break;
	case LUA_TNUMBER:
        OutBugger << PrevStr << lua_tonumber(L, Index);
        break;
	case LUA_TSTRING:
		{
			OutBugger << PrevStr << lua_tostring(L,Index);
		}
		break;
	case LUA_TTABLE:
	{
		lua_checkstack(L, lua_gettop(L) + 2);
		OutBugger << PrevStr << "{";	


		//size_t len = lua_objlen(L,Index);  //array,table
		//if (len > 0)
        {
            lua_pushnil(L);
            Space = PrevStr + tabkey;

            while (lua_next(L, Index) != 0)
            {
                DumpLuaValue(L, lua_gettop(L) - 1, OutBugger, Space, ForceSameLine, Deep);
                OutBugger << ((" : "));
                DumpLuaValue(L, lua_gettop(L), OutBugger, Space, true, Deep);
                lua_pop(L, 1);
            }

        }
		OutBugger << "\r\n"<< PrevStr << "}";	
	}
		break;
	case LUA_TFUNCTION:
		{
			lua_pushvalue(L, Index);
			lua_Debug Info;
			lua_getinfo(L, ">Sn", &Info);
			OutBugger <<  "Function="
					<< Info.namewhat  
//<<  Info.name 
<< "@" 
<< Info.short_src<< "(" << Info.linedefined << "-" << Info.lastlinedefined << ")" ;	
		}
		break;
	case LUA_TUSERDATA:
		OutBugger << PrevStr << "UserData";
		break;
	case LUA_TTHREAD:
		OutBugger << PrevStr << "Thread";
		break;
	//case LUA_TINTEGER:
	//	Temp.Format(("%s%lld"), PrevStr, luaointeger(L, Index));
	//	OutBugger << (Temp);
	//	break;
	default:
		//if (Type >= CLuaBaseMetaClass::CLASS_ID)
		//{
		//	CLuaBaseMetaClass* pObject = *((CLuaBaseMetaClass**)luaouserdata(L, Index));
		//	if(pObject)
		//	{
		//		CEasyString ObjDump;
		//		pObject->Dump(ObjDump);
		//		Temp.Format(("%s%s"), PrevStr, (LPCTSTR)ObjDump);
		//		OutBugger << (Temp);
		//	}
		//}		
		//else
		{
			OutBugger << "Unknow";
		}
	}	
}


int LuaPrint(lua_State* L)
{
	int ParamCount = lua_gettop(L);
	if (ParamCount)
	{
		std::stringstream ss;
		for (int i = 1; i <= ParamCount; i++)
		{
			DumpLuaValue(L, i, ss, (""), false, 0);
		}

		std::cout << ss.str() << std::endl;	
	}
	return 0;
}

int LuaPrintIdx(lua_State* L, int idx)
{
	std::stringstream ss;
	lua_pushvalue(L,idx);
	DumpLuaValue(L, lua_gettop(L), ss, (""), false, 0);
	std::cout << ss.str() << std::endl;	
	
	lua_pop(L,1);
	
	return 0;
}


static const struct luaL_Reg printlib [] = {
  {"print", LuaPrint},
  {NULL, NULL} /* end of array */
};

//重定向print
void ReSetLuaPrint(lua_State *L)
{
  lua_getglobal(L, "_G");
  luaL_register(L, NULL, printlib); // for Lua versions < 5.2
  //luaL_setfuncs(L, printlib, 0);  // for Lua versions 5.2 or greater
  lua_pop(L, 1);
}
/*
		lua_pushcfunction(GetLuaState(), LuaPrint);
		lua_setglobal(GetLuaState(), "print");
*/

class cc 
{
	public :
		cc():v1(0x123456),v2(0x10000) {}
		~cc() {}
	public :
		int v1;
		int v2;
	public :
		cc& testRetRef(){
			return *this;
		}
		int testRet(){
			return *(int*)this;
		}
		cc* testThisPtr(){
			return this;
		}
		int* testThis(){
			return (int*)this;
		}
};


void tttt()
{
	cc g;
	g.testRetRef();
	g.testRet();
	g.testThisPtr();
	g.testThis();
}


extern int testGameLua(lua_State *L);
extern int testDefault(lua_State *L);
extern void testMapPath();
extern void testAVL();
extern int main_bind();
extern int main_bind01();


int test000()
{
    lua_State* L= lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

    //debugTraceBack(L);

	//testHandler(L);
	//luaL_error(L, "test err:%s", "xxx");


	///< 加载相关库文件
	luaL_openlibs(L);
    
	ReSetLuaPrint(L);

    //testGameLua(L);

    ///< 加载lua文件
    std::string sf = dir_scripts + "/test000.lua";
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

    testDefault(L);
	lua_register(L, "TestFunc", luaTestFunc);



    //lua_getglobal(L, "g_tt");
	//LuaPrintIdx(L,-1);
	//lua_pop(L,1);

	
	//测试方法
    //TestCClosure(L);
    //GetObjectIndexData1(L);
    //threadTest(L);
	

	//testtable(L); //todo


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
        return -3;
    }

	system("pause");
	return 0;
}
int main1(int argc, char* argv[])
{
	//test000(); // 元表测试
	test001(); // 有打印 Proto
	//test002(); //c++/lua 数据相互使用
	//test003();
    
	//testMapPath();
	//testAVL();


	//main_bind();
	//main_bind01();
	//tttt();



	system("pause");
	return 0;
}

int main_helloworld(int argc, char* argv[])
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
    std::string sf = dir_scripts + "/helloworld001.lua";
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
        return -3;
    }

	system("pause");
	return 0;
}

extern void test_table();
extern void test_matetable();
int main(int argc, char* argv[])
{
	//main1(argc,argv);
	//main_helloworld(argc,argv);
	//test_table();//to test

	test_matetable();
	return 0;
}







