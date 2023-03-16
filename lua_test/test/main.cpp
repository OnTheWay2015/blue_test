#include <iostream>
#include <sstream>

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
void  lua_insert(lua_State *L, int idx)
��ָ��λ���ϵ�����Ԫ���Կ���һ���ռ�۵Ŀռ䣬Ȼ��ջ��Ԫ���Ƶ���λ��

for (int i = 1; i <= 5; ++i)
      lua_pushnumber(i);

ջ��Ԫ�أ����������ϣ� 1 2 3 4 5

lua_insert(L, 3)

ջ��Ԫ�أ����������ϣ� 1 2 5 4 3

*/





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
        while(lua_next(L, idx) != 0){// 1), ��һ�� nil ��ȥ��,ѹ��һ�� key_value
            int keyType = lua_type(L, -2); //key ��  -2�� value �� -1
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
	lua_State *L = lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

	///< ������ؿ��ļ�
	luaL_openlibs(L);

	///< ����lua�ļ�
	std::string sf = dir_scripts + "/test001.lua";
    if (luaL_loadfile(L, sf.c_str()))
    {
        cout << "Lua �ļ�����ʧ��" << endl;
        return -2;
    }

    {

		//�ű����غ�������һ�� Closure ��ڷ���,ѹ��ջ��.���Ե�ǰջ top ����һ���������ʵ��,ͨ����������ҵ� Proto 
		p_func_proto(clvalue( L->top - 1)->l.p);

		///< ִ��lua�ļ� ��ڷ���
		//LUA_API int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) {
		if (lua_pcall(L, 0, 0, 0))
		{
			cerr << lua_tostring(L, -1) << endl;
		}
		else
		{
			/// ��ȡ lua mystr ֵ
			lua_getglobal(L, "mystr");
			string str = lua_tostring(L, -1);
			string str2 = lua_tostring(L, -1);
			cout << str << "  " << str2 << endl;
			lua_pop(L, 1); 

			///< ��ȡ lua myTable ��������
			lua_getglobal(L, "myTable");
			lua_getfield(L, -1, "name");
			cout << lua_tostring(L, -1) << endl;
			lua_pop(L, 1); 


			lua_getfield(L, -1, "id");
			cout << lua_tonumber(L, -1) << endl;
			lua_pop(L, 1); 


            int type = lua_type(L, -1);//dump ��ӡ myTable
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

			//lua  myAry ����
			lua_getglobal(L, "myAry");
			auto n = lua_objlen(L, -1);
			
			//��ӡ����
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(L, i);  //��ջ����ѹ��i
				lua_gettable(L, -2);  //tableλ��-2��λ��, ��ȡtable[i] �� top��
				//lua_rawget(L, -2);  //lua_gettableҲ������lua_rawget���滻
				cout << lua_tostring(L, -1) << endl;
				lua_pop(L, 1); //����top��ȡ����table[i]
			}

			//�޸� ����
			for (int i = n+1; i <= n+2; ++i) {
				lua_pushnumber(L, i);
				char buf[256];
				sprintf(buf, "hehe%d", i);
				lua_pushstring(L, buf);
				//lua_settable(L, -3);// table λ�� -3 ��λ��; -1,��Ҫ�޸�ֵ�� table ���±�;-2 ����ֵ
				lua_rawset(L, -3);
			}

			//��ӡ�޸ĺ�����
			n = lua_objlen(L, -1);
			for (int i = 1; i <= n; ++i) {
				lua_pushnumber(L, i);  //��ջ����ѹ��i
				lua_gettable(L, -2);  //tableλ��-2��λ��, ��ȡtable[i] �� top��
				//lua_rawget(L, -2);  //lua_gettableҲ������lua_rawget���滻
				cout << lua_tostring(L, -1) << endl;
				lua_pop(L, 1); //����top��ȡ����table[i]
			}









			///< ���� lua ���� print_hello
			lua_getglobal(L, "print_hello");
			lua_pcall(L, 0, 0, 0);

			///< ���� lua ���� _add
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

void testtable(lua_State* L)
{
//�鿴ջ L->top - L->stack 
    lua_newtable(L);
    //lua_pushinteger(L, 123);
    lua_pushstring(L, "123");
    lua_newtable(L);
    lua_settable(L, -3);


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
		
	lua_atpanic(L, panichandler);//������panic���������ؾɵ�panic����
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
		lua_checkstack(L, lua_gettop(L) + 2);
		OutBugger << PrevStr << "{";	

		lua_pushnil(L);
		Space =PrevStr +  tabkey ;
		
		while (lua_next(L, Index) != 0)
		{
			DumpLuaValue(L, lua_gettop(L) - 1, OutBugger, Space, ForceSameLine, Deep);
			OutBugger << ((" : "));
			DumpLuaValue(L, lua_gettop(L), OutBugger, (""), true, Deep);
			lua_pop(L, 1);
		}
		OutBugger << "\r\n"<< PrevStr << "}";	
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



static const struct luaL_Reg printlib [] = {
  {"print", LuaPrint},
  {NULL, NULL} /* end of array */
};

//�ض���print
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




extern int testGameLua(lua_State *L);
extern int testDefault(lua_State *L);
extern void testMapPath();
extern void testAVL();
int main(int argc, char* argv[])
{
	//test001(); // �д�ӡ Proto
	//return 0;	
	//test002();
	//test003();
    
	//testMapPath();
	//testAVL();
	//return 0;



    lua_State* L= lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}

    //debugTraceBack(L);

	//testHandler(L);
	//luaL_error(L, "test err:%s", "xxx");


	///< ������ؿ��ļ�
	luaL_openlibs(L);
    
	ReSetLuaPrint(L);

    testGameLua(L);

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

    testDefault(L);
	lua_register(L, "TestFunc", luaTestFunc);
	
	//���Է���
    //TestCClosure(L);
    //GetObjectIndexData1(L);
    //threadTest(L);
	testtable(L);


	lua_pushcfunction(L,PcallErrorFunc);
	int errfunc = lua_gettop(L);
	
	///ָ�� lua ��ں��� run 
    lua_getglobal(L, "run_main");
    //ִ��
	//if (lua_pcall(L, 0, 0, 0))
	if (lua_pcall(L, 0, 0, errfunc)) //������ errfunc ��û������,����ʧ�ܺ��ջ�᲻һ��.������ errfunc ʱ,ջ��Ĵ�����Ҫ�� errfunc �ﴦ��
    {
		cout << "���ýű���ڷ���ʧ��" << endl;
		//cout << lua_tostring(L,-1) << endl;  //û������ errfunc ʱ,ջ��Ĵ�����Ҫ�����ﴦ��
        return -3;
    }

	system("pause");
	return 0;
}






