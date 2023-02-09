#include <iostream>

extern "C" { // lua.dll����c�⣬ʹ��ʱҪ�� extern "C",  ��� lua�ⰴ c++������ľ���  __declspec(dllimport)/dllexport ����

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;



static string dir_scripts = "../../test/scripts";


int n_act01(lua_State* L)
{
	//lua_gettop ��ȡ��ջ��������ֵ����ʱջ��������ֵ��С����ջ��Ԫ�صĸ���
	int n = lua_gettop(L);
	double sum = 0;
	//ʹ��ѭ������վ�����е�Ԫ�أ�ͨ��lua_tonumberȡ��ջ�ڵ�ֵ��Ȼ�������Ӳ�����
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//��������ֵ������Lua����Ҫ���ص�ֵ��ѹ��ջ����ʱ��ʱջ��7�����ݣ��ο�ջ������ͼLog index 2
	//lua_pushnumber(L, sum / n);//average
	lua_pushnumber(L, sum);//sum
	
	//����lua�����򣬷���1��ֵ, �ڽű����ʹ��
	return 1;
}


int n_act02(lua_State* L)
{
	//lua_gettop ��ȡ��ջ��������ֵ����ʱջ��������ֵ��С����ջ��Ԫ�صĸ���
	int n = lua_gettop(L);
	double sum = 0;
	//ʹ��ѭ������վ�����е�Ԫ�أ�ͨ��lua_tonumberȡ��ջ�ڵ�ֵ��Ȼ�������Ӳ�����
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//��������ֵ������Lua����Ҫ���ص�ֵ��ѹ��ջ����ʱ��ʱջ��7�����ݣ��ο�ջ������ͼLog index 2
	lua_pushnumber(L, sum / n);//average
	//lua_pushnumber(L, sum);//sum
	
	//����lua�����򣬷���2��ֵ��lua���ǿ����ò�������������ֵ
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
//lua��ʹ�� c++ ��ע���ģ��
void test003()
{
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< ������ؿ��ļ�
	luaL_openlibs(pState);

	luaL_register(pState, "testlib", testlib);
	
	///< ����lua�ļ�
	std::string sf = dir_scripts + "/test003.lua";
	if (luaL_loadfile(pState, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
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
	//code3   lua_gettop ��ȡ��ջ��������ֵ����ʱջ��������ֵ��С����ջ��Ԫ�صĸ���
	int n = lua_gettop(L);
	double sum = 0;
	//code4 ʹ��ѭ������վ�����е�Ԫ�أ�ͨ��lua_tonumberȡ��ջ�ڵ�ֵ��Ȼ�������Ӳ�����
	for (int i = 1; i <= n; ++i)
	{
		auto v = lua_tonumber(L, i);
		sum += v;
		cout << "argn["<<n<<"]  idx["<<i<<"] v["<<v<<"]" << endl;
	}
	//code5 ��������ֵ������Lua����Ҫ���ص�ֵ��ѹ��ջ����ʱ��ʱջ��7�����ݣ��ο�ջ������ͼLog index 2
	lua_pushnumber(L, sum / n);//average
	lua_pushnumber(L, sum);//sum
	//code6 ����lua�����򣬷���2��ֵ��lua���ǿ����ò�������������ֵ
	return 2;

}





//lua��ʹ�� c++ ��ķ���
void test002()
{
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return;
	}

	///< ������ؿ��ļ�
	luaL_openlibs(pState);

	///< ����lua�ļ�
	std::string sf = dir_scripts + "/test002.lua";
	if (luaL_loadfile(pState, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
		return;
	}

	//code1 lua_register ע�ắ���� Lua ������ C++ �������а󶨡�
	//��ʵ��������lua_pushcfunction����c++�ж���ĺ���ѹ��ջ�У�
	//Ȼ��lua_setglobal������ջ����Ԫ�ض�Ӧ��ֵ�������Ϳ��԰�lua������ջ����c++�����������ù�ϵ��
	lua_register(pState, "average", Average);
	//code2  ���ز�ִ��lua�ű�����ʱlua�еĺ���average��ִ�У�ͬʱ��ջ��ѹ��5���������ο�ջ������ͼLog index 1
	//luaL_dofile(L, "testLC.lua");
	 if (lua_pcall(pState, 0, 0, 0))
	 {
		 cout << lua_tostring(pState, -1) << endl;
		 return;
	 }
	 
	 
	 lua_close(pState);
}

//���� Table
bool DumpTable(lua_State* L, int idx)
{
    try{
        lua_pushnil(L);
/*
lua_next() ��������Ĺ��������ǣ�
1) �ȴ�ջ������һ�� key, ������ʹ�� lua_next ֮ǰ����ѹ��һ�� nil. lua_pushnil(L);
2) ��ջָ��λ�õ� table ��ȡ��һ�� key-value���Ƚ� key ��ջ�ٽ� value ��ջ
3) ����� 2 ���ɹ��򷵻ط� 0 ֵ�����򷵻� 0�����Ҳ���ջ��ѹ���κ�ֵ
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
 


//c++ ʹ�� lua���������
int test001()
{
	///< ����lua�������ʼ��
	//lua_State *pState = luaL_newstate();
	lua_State *pState = lua_open();

	if (nullptr == pState)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< ������ؿ��ļ�
	luaL_openlibs(pState);

	///< ����lua�ļ�
	std::string sf = dir_scripts + "/test001.lua";
    if (luaL_loadfile(pState, sf.c_str()))
    {
        cout << "Lua �ļ�����ʧ��" << endl;
        return -2;
    }

    {

		//�ű����غ�������һ�� Closure ��ڷ���,ѹ��ջ��.���Ե�ǰջ top ����һ���������ʵ��,ͨ����������ҵ� Proto 
		p_func_proto(clvalue( pState->top - 1)->l.p);

		///< ִ��lua�ļ� ��ڷ���
		//LUA_API int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
		if (lua_pcall(pState, 0, 0, 0))
		{
			cerr << lua_tostring(pState, -1) << endl;
		}
		else
		{
			/// ��ȡ lua mystr ֵ
			lua_getglobal(pState, "mystr");
			string str = lua_tostring(pState, -1);
			string str2 = lua_tostring(pState, -1);
			cout << str << "  " << str2 << endl;
			lua_pop(pState, 1); 

			///< ��ȡ lua myTable ��������
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


			//lua  myAry ����
			lua_getglobal(pState, "myAry");
			auto n = lua_objlen(pState, -1);
			
			//��ӡ����
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);  //��ջ����ѹ��i
				lua_gettable(pState, -2);  //tableλ��-2��λ��, ��ȡtable[i] �� top��
				//lua_rawget(pState, -2);  //lua_gettableҲ������lua_rawget���滻
				cout << lua_tostring(pState, -1) << endl;
				lua_pop(pState, 1); //����top��ȡ����table[i]
			}

			//�޸� ����
			for (int i = n+1; i <= n+2; ++i) {
				lua_pushnumber(pState, i);
				char buf[256];
				sprintf(buf, "hehe%d", i);
				lua_pushstring(pState, buf);
				//lua_settable(L, -3);
				lua_rawset(pState, -3);
			}

			//��ӡ�޸ĺ�����
			n = lua_objlen(pState, -1);
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(pState, i);  //��ջ����ѹ��i
				lua_gettable(pState, -2);  //tableλ��-2��λ��, ��ȡtable[i] �� top��
				//lua_rawget(pState, -2);  //lua_gettableҲ������lua_rawget���滻
				cout << lua_tostring(pState, -1) << endl;
				lua_pop(pState, 1); //����top��ȡ����table[i]
			}









			///< ���� lua ���� print_hello
			lua_getglobal(pState, "print_hello");
			lua_pcall(pState, 0, 0, 0);

			///< ���� lua ���� _add
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
lua_newuserdata  ��������ָ���Ĵ�С����һ���ڴ棬����Ӧ�� userdatum�ŵ�ջ��,�������ڴ��ĵ�ַ��
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
	buffer = lua_touserdata(L, lua_upvalueindex(1)); //�� lua_pushcclosure ����������

	cout <<"buffer store string:" << (char*)buffer <<endl;

	int a=0x123;
	


	return 0;
}
/*
int lua_pushcclosure(lua_State *L,lua_CFunction func,int n )
	����n�Ƕ�ջ����ֵ�ø�������Щֵ�����Ƶ�closure�У�func��ʹ��closure�ĺ�����
	����������ú�����ջ��n��ֵ����ѹ��һ��closure��ջ�С�
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
	memcpy(buffer, str,strlen(str) ); //ʹ�� buffer,���� str,δ��������
	lua_pushcclosure(L, function, 1);
		/*
			���� n=1,��һ����������,����ϱ��Ǹ� lua_newuserdata ���Ƶ� closuer,֮����ջ�ﵯ��.
			���Ժ����ӡջ��ʱ,�������һ�� 
		*/
	cout << "top:"<< L->top - L->base  << endl;

	stackDump(L,1);
	
	
	lua_pcall(L, 0, 0, 0); //ִ�� cclosure
	
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
ʹ�� restorestack ���������λջ�ϵĵ�ַ,����Ϊ����ջ���ڴ��ַ�ǻ���������ջ�Ĵ�С���仯��
�����ַ�ǲ����ܵģ���Ӧ�ü�סһ���������
 savestack �� restorestack ���������������������ġ�

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
    lua_pushinteger(L, 10); //Ҫ����Ĳ���

    // �������Эͬ����, ����һ������
    // ���ﷵ�� bRet ��Ӧ��ǰ L->status : 0,OK/ 1,LUA_YIELD / 2,LUA_ERRRUN	

    auto bRet = lua_resume(L, 1); // �ű����� Func1 �����õ� coroutine.yield
    cout << " L->status bRet:" << bRet << endl;

	// �ű�  coroutine.yield ����Ĳ�����ѹ��ջ��,����Ѳ�������ѹ��
    // ��ӡL1ջ�в����ĸ���
    cout << "Element Num:" << lua_gettop(L) << endl;

    // ��ǰ yield ���ص�����ֵ
    cout << "Value 1:" << lua_tointeger(L, -2) << endl;
    cout << "Value 2:" << lua_tointeger(L, -1) << endl;

    // �ٴ�����Эͬ����, ����1������
    // ���ﷵ��0
    lua_pushinteger(L, 123); //Ҫ����Ĳ���, �ڽű���ͨ�� coroutine.yield ����ֵ���� 
    bRet = lua_resume(L, 1); //�ٴε��ȵ� Lua�߳� ֮ǰ yield �� 
    cout << "bRet:" << bRet << endl;
    cout << "Element Num:" << lua_gettop(L) << endl;
    cout << "Value 1:" << lua_tointeger(L, -1) << endl;
}








void testMetatable(lua_State* L)
{
    // ��ȡ lua myTable 
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
	int n = lua_gettop(L); //��ǰջѹ�˼�������(����)

	lua_getmetatable(L,1);
	
	return 0; //0������ֵ
}


//typedef char *C,*CCC;

extern void testMapPath();
extern void testAVL();
int main(int argc, char* argv[])
{
	//test001(); // �д�ӡ Proto
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

	///< ������ؿ��ļ�
	luaL_openlibs(L);

	///< ����lua�ļ�
	std::string sf = dir_scripts + "/test004.lua";
	//std::string sf = dir_scripts + "/test001.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }

	cout << "Lua �ļ�����["<< sf <<"] �ɹ�" << endl;

    //p_func_proto(clvalue(L->top - 1)->l.p); //dump Ĭ�ϵıհ�����
    
	//ִ�нű����غ�,Ĭ�ϵıհ�����,��ʼ����ǰ�ű�����
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "�ű���ʼ��ʧ��" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	lua_register(L, "TestFunc", luaTestFunc);
	
	//���Է���
    //TestCClosure(L);
    //GetObjectIndexData1(L);
    //testMetatable(L);
    threadTest(L);

	///ָ�� lua ��ں��� run 
    lua_getglobal(L, "run_main");
    //ִ��
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "���ýű���ڷ���ʧ��" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	system("pause");
	return 0;
}



