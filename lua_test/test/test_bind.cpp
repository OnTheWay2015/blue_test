#include <iostream>
#include <sstream>

extern "C" { // lua.dll����c�⣬ʹ��ʱҪ�� extern "C",  ��� lua�ⰴ c++������ľ���  __declspec(dllimport)/dllexport ����

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

#include <windows.h>//Sleep

using namespace std;

/*
//���������Ƶ� 
 
template <typename Callee, typename P1>  // P1 �ǲ���,�����Ǵ�һ�������� Call, ���в����ʹ������,Callͨ�����벻ͬ�� Callee::* func ,ʵ�ֶ�̬����
static int Call(Callee& callee, LUA_WRAP_CALL_RETURN_TYPE(Callee::* func)(P1) const, lua_State* L, int index, CLuaThread* pThreadInfo) //��̬���ַ���
{
	luaL_argassert(1, index + 0);

	LUA_WRAP_RETURN_PUSH_OPERATION((callee.*func)(
		Get(TypeWrapper<P1>(), L, index + 0)
		));
}
...

inline unsigned short	Get(TypeWrapper<unsigned short>, lua_State* L, int idx) //��̬���ַ���,������,��luaջ��ת�������� 
{
	return static_cast<unsigned short>(lua_tonumber(L, idx));
}
...

template<class T> struct TypeWrapper {};

*/

static string dir_scripts = "../../test/scripts";
extern int LuaPrintIdx(lua_State* L, int idx);

void SetMetaClass(lua_State * L);
void InitClassMember(lua_State* L);


//todo __gc
class Person
{
public:
    Person() 
    {
        std::cout << "Person create" << std::endl;
    }

    ~Person() 
    {
        std::cout << "Person dispose" << std::endl;
    }
public:
    std::string str;
    int value;
public:
    int Act(lua_State *L)
    {
        std::cout << "Person act" << std::endl;
        return 0;
    }
//protected:
public:
static int Create(lua_State *L)
{
    //lua_pushstring(L, "Create act!");
	lua_newtable(L); // ����һ�� table ����ʵ������
    LuaPrintIdx(L,-1); 
    lua_pushstring(L, "__this");
    size_t nbytes = sizeof(Person*)/*���� this ָ���ֵַ*/ + sizeof(Person) /*ʵ�����ݿռ�*/;
    char* a = (char*)lua_newuserdata(L,nbytes);
	auto pObject = a+sizeof(Person*);//ʵ�����ݵ�ַ
	::new(pObject) Person();
   
    auto obj = (Person*)(pObject); 
    obj->value = 123;
    obj->str ="999";

    memcpy(a,&pObject,sizeof(Person*));//���� this ָ���ֵַ

	lua_settable(L, -3);
    
   

    LuaPrintIdx(L,-1); 



 
    InitClassMember(L);
	
    SetMetaClass(L);//����Ԫ��	

    LuaPrintIdx(L,-1); 

	return 1;
}

};
class Student 
{
	public:
Student() {
			std::cout<<"Student create" << std::endl;
}
~Student() {
			std::cout<<"Student dispose" << std::endl;
}
	public:
static Student* Create(lua_State *L)
{
	return new Student();
}
	public:
		void Act(lua_State *L)
		{
			std::cout<<"Student act" << std::endl;
		}
};


//static const struct luaL_Reg stuentlib_f [] = {
//    {"Create", Student::Create},
//    {"Act", Student::Act},
//    {NULL, NULL}
//};
//int luaopen_student (lua_State *L) {
//    luaL_newlib(L, stuentlib_f);
//    return 1;
//}


void regLuaclass(lua_State* L)
{
}


static int DoGarbageCollect(lua_State* L)
{
    std::cout << "DoGarbageCollect act" << std::endl;
    return 0;
}
static int DoGetMember(lua_State* L)
{
    std::cout << "DoGetMember act" << std::endl;
    if (lua_type(L, 1) != LUA_TTABLE)
    {
        std::cout << "DoGetMember err need table" << std::endl;
        return 0;
    }
    if (lua_type(L, 2) != LUA_TSTRING)
    {
        std::cout << "DoGetMember need key" << std::endl;
        return 0;
    }
    std::cout << "DoGetMember act key:"<< lua_tostring(L, 2) << std::endl;
   
 
    //LuaPrintIdx(L,1); 
    lua_getfield(L, 1, "__this");
    if (lua_type(L, -1) != LUA_TUSERDATA)
    {
        std::cout << "DoGetMember err need this" << std::endl;
        lua_pop(L, 1);
        return 0;
    }

    //auto pBuff = (char*)lua_touserdata(L, -1);
    //Person* p = dynamic_cast<Person*>(*(Person**)pBuff);
    //if (!p)
    //{
    //    std::cout << "DoGetMember err this need Person" << std::endl;
    //    lua_pop(L, 1);
    //    return 0;
    //}


    if (lua_isstring(L, 2))
    {
        std::string Temp;
        auto szPropertyName = lua_tostring(L, 2);
        //if (LUA_SCRIPT_CODE_PAGE != CEasyString::SYSTEM_STRING_CODE_PAGE)
        //{
        //	LuaStrToSystemStr(lua_tostring(L, 2), Temp);
        //	szPropertyName = Temp;
        //}

        lua_getfield(L, 1, "__bindfuncs");
        if (lua_type(L, -1) != LUA_TTABLE)
        {
            std::cout << "DoGetMember no __bindfuncs " << std::endl;
            lua_pop(L, 2);
            return 0;
        }

        lua_pushvalue(L, 2);//act key
        lua_gettable(L, -2);// �� __bindfuncs �� �����ö�Ӧkey��ֵ,ѹջ,��ǰӦ���Ǹ��հ����� 
        if (lua_isnil(L, -1))
        {
            std::cout << "DoGetMember __bindfuncs no member[" << szPropertyName << "]" << std::endl;
            lua_pop(L, 3);
            return 0;
        }
        return 1;
    }
    return 0;
}


static int DoSetMember(lua_State* L)
{
    std::cout << "DoSetMember act" << std::endl;
    return 0;
}

void SetMetaClass(lua_State * L)
{
	const char* ClassName = "Person";
	luaL_getmetatable(L, ClassName);
	if (lua_isnil(L, -1))
    {
        lua_pop(L, 1); //���� nil
        
        auto ClassName = "Person";
        luaL_newmetatable(L, ClassName);  // ����һ�� meta ��ѹջ, ������ȫ�ֱ� LUA_REGISTRYINDEX ��

        lua_pushstring(L, "__gc");
        lua_pushcclosure(L, &DoGarbageCollect, 0);
        lua_settable(L, -3); //���ڴ����� meta ����

        lua_pushstring(L, "__index");
        lua_pushcclosure(L, &DoGetMember, 0);
        lua_settable(L, -3);//���ڴ����� meta ���� 

        lua_pushstring(L, "__newindex");
        lua_pushcclosure(L, &DoSetMember, 0);
        lua_settable(L, -3);//���ڴ����� meta ����

        lua_pop(L, 1); //�½���Ԫ���ջ

        luaL_getmetatable(L, ClassName); //ѹ��Ԫ��
    }
	lua_setmetatable(L, -2); //����Ԫ�� ��ǰ -2 �Ķ�����
}


unsigned char* GetFirstUpValueAsUserData(lua_State* L)
{
    void* buffer;
    buffer = lua_touserdata(L, lua_upvalueindex(1));
    return (unsigned char*)buffer;
}




typedef int (Person::*FFFF)(lua_State * L);
static int DirectCallMemberDispatcher(lua_State* L)
{
/*
    Person* xx;
    auto ff = &Person::Act;
    (xx->*ff)(); 
    
    void (*q)() = nullptr; 
    void (Person::*qq)() = &Person::Act; 
    FFFF qqq = &Person::Act; 
    void* ccc = (void*)(&qq); 
    auto ffff = (FFFF*)ccc;
*/


    int Type = lua_type(L, -1);
    if (Type != LUA_TTABLE)
    {
        //err
        return 1; 
    }

    LuaPrintIdx(L, -1);
    lua_getfield(L,-1,"__this");
    auto pBuff = (char*)lua_touserdata(L, -1);
    Person* xx = *(Person**)pBuff;//�Ѵ�ĵ�ֵַתΪָ��
    unsigned char* buffer = GetFirstUpValueAsUserData(L);
    auto ffff =reinterpret_cast<FFFF*>(buffer); 
    auto argsn = (xx->**ffff)(L);  
    return argsn;
    //return 1; 
}


typedef int (*SFFFF)(lua_State * L);
static int DirectCallDispatcher(lua_State* L)
{
    unsigned char* buffer = GetFirstUpValueAsUserData(L);
    auto ffff =reinterpret_cast<SFFFF*>(buffer); 
    auto argsn = (**ffff)(L);  
    return argsn;
}


template<class FUNC >
//void RegisterMetaCFun(lua_State * L, const char* funcName, lua_CFunction function, void* func, unsigned int sizeofFunc) //�����ﴴ��һ���հ����õĲ��� userdata
void RegisterMetaCFun(lua_State * L, const char* funcName, lua_CFunction function, FUNC func, unsigned int sizeofFunc) //�����ﴴ��һ���հ����õĲ��� userdata
{
	lua_pushstring(L, funcName);

	unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeofFunc); //�հ�����, ʵ��Ҫʹ�õķ���ָ��

	memcpy(buffer, &func, sizeofFunc);

	//int lua_pushcclosure(lua_State *L,lua_CFunction func,int n ) ���� n �Ƕ�ջ����ֵ�ø���,��Щֵ�����Ƶ� closure ��, ������ closure ʱ, ��Щֵ��ͨ�� lua_upvalueindex ȡ�� 
	lua_pushcclosure(L, function, 1); //���洴���� lua_newuserdata �Ḵ�Ƶ� closure ��, ������ջ 
	
	//�հ��������õ� table ��. [funcName, closure] 
	lua_settable(L, -3); 
}

template<class FUNC >
void RegisterStaticCFun(lua_State * L, const char* gname, const char* funcName, lua_CFunction function, FUNC func, unsigned int sizeofFunc) //�����ﴴ��һ���հ����õĲ��� userdata
{
    lua_getglobal(L, gname);//gname table��ѹջ
    int Type = lua_type(L, -1);
    if (Type != LUA_TTABLE)
    {
        lua_pop(L, 1);
        lua_newtable(L);
        lua_setglobal(L, gname);
        lua_getglobal(L, gname);
    }

    lua_pushstring(L, funcName);

    unsigned char* buffer = (unsigned char*)lua_newuserdata(L, sizeofFunc); //�հ�����, ʵ��Ҫʹ�õķ���ָ��

    memcpy(buffer, &func, sizeofFunc);

    //int lua_pushcclosure(lua_State *L,lua_CFunction func,int n ) ���� n �Ƕ�ջ����ֵ�ø���,��Щֵ�����Ƶ� closure ��, ������ closure ʱ, ��Щֵ��ͨ�� lua_upvalueindex ȡ�� 
    lua_pushcclosure(L, function, 1); //���洴���� lua_newuserdata �Ḵ�Ƶ� closure ��, ������ջ 

    //�հ��������õ� table ��. [funcName, closure] 
    lua_settable(L, -3);
    
    lua_pop(L, 1); //gname table���ջ
}

void RegisterCreateFunction(lua_State* L)
{
    RegisterStaticCFun(L, "Person","NEW", DirectCallDispatcher,&Person::Create, sizeof(&Person::Create));
}
void RegisterMemberFunctions(lua_State* L)
{
    //RegisterMetaCFun(L, "Create", DirectCallMemberDispatcher,&Person::Create, sizeof(&Person::Create));
    RegisterMetaCFun(L, "Act", DirectCallMemberDispatcher,&Person::Act, sizeof(&Person::Act));


    //lua_pushstring(L, "Act");
    //lua_pushcclosure(L, Person::Act, 0);
    //lua_settable(L, -3);//���ڴ����� meta ����

}
void InitClassMember(lua_State* L)
{
	lua_pushstring(L, "__bindfuncs"); //�󶨷���������
	lua_newtable(L);

	RegisterMemberFunctions(L);

    lua_settable(L, -3);
    
	
    //lua_setiuservalue(L, -2, 1);
}

//void tttt(lua_State* L)
//{
//	lua_newtable(L); // ����һ�� table ����ʵ������
//   
//    lua_pushstring(L, "__this");
//    size_t nbytes = sizeof(Person*)/*���� this ָ��*/ + sizeof(Person) /*ʵ�����ݿռ�*/;
//    char* a = (char*)lua_newuserdata(L,nbytes);
//	auto pObject = a+sizeof(Person*);//ʵ�����ݵ�ַ
//	::new(pObject) Person();
//	lua_settable(L, -3);
//   
// 
//	SetMetaClass(L);//����Ԫ��	
//
//    InitClassMember(L);
//}

void testGC(lua_State* L)
{
    lua_newtable(L);
	SetMetaClass(L);
    lua_pop(L,1);
}


int main_bind()
{
    lua_State* L= lua_open();

	if (nullptr == L)
	{
		cout <<"lua init fail"<< endl;
		return -1;
	}



	///< ������ؿ��ļ�
	luaL_openlibs(L);
    

    ///< ����lua�ļ�
    std::string sf = dir_scripts + "/test005.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }
	cout << "Lua �ļ�����["<< sf <<"] �ɹ�" << endl;
	//tttt(L);
	RegisterCreateFunction(L);
	regLuaclass(L);
    
	//ִ�нű����غ�,Ĭ�ϵıհ�����,��ʼ����ǰ�ű�����
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "�ű���ʼ��ʧ��" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	
    testGC(L);

	//lua_pushcfunction(L,PcallErrorFunc);
	//int errfunc = lua_gettop(L);
	
	///ָ�� lua ��ں��� run 
    lua_getglobal(L, "run_main");
    //ִ��
	if (lua_pcall(L, 0, 0, 0))
	//if (lua_pcall(L, 0, 0, errfunc)) //������ errfunc ��û������,����ʧ�ܺ��ջ�᲻һ��.������ errfunc ʱ,ջ��Ĵ�����Ҫ�� errfunc �ﴦ��
    {
		cout << "���ýű���ڷ���ʧ��" << endl;
		cout << lua_tostring(L,-1) << endl;  //û������ errfunc ʱ,ջ��Ĵ�����Ҫ�����ﴦ��
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






