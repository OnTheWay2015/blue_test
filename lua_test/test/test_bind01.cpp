
extern "C" { // lua.dll����c�⣬ʹ��ʱҪ�� extern "C",  ��� lua�ⰴ c++������ľ���  __declspec(dllimport)/dllexport ����

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}


 
#include <iostream>
#include<string>
using namespace std;


static string dir_scripts = "../../test/scripts";
class Student
{
public:
    Student();
    ~Student();
    string getName();
    void setName(string name);
    int getAge();
    void setAge(int age);
    void print();
    
private:
    string name;
    int age;
};

 

Student::Student()
:name("default")
{
    cout<<"Student Contructor called"<<endl;
}

Student::~Student()
{
    cout<<"Student Destructor called"<<endl;
}

string Student::getName()
{
    return name;
}

void Student::setName(string name)
{
    this->name = name;
}

int Student::getAge()
{
    return age;
}

void Student::setAge(int age)
{
    this->age = age;
}

void Student::print()
{
    cout<<"My name is: "<<name<<", and my age is "<<age<<endl;
}




int l_setName(lua_State * L)
{
     Student **s = (Student**)luaL_checkudata(L,1,"cc.Student");
    luaL_argcheck(L, s != NULL, 1, "invalid user data");

 

    luaL_checktype(L, -1, LUA_TSTRING);

    std::string name = lua_tostring(L, -1);
    (*s)->setName(name);
    return 0;
}

int l_print(lua_State* L)
{
    Student **s = (Student**)lua_touserdata(L,1);
    luaL_argcheck(L, s != NULL, 1, "invalid user data");
    (*s)->print();

    return 0;
}
int l_gc(lua_State* L)
{
    Student **s = (Student**)lua_touserdata(L,1);
    (*s)->~Student();
    return 0;
}

int newStudent(lua_State * L)
{
    Student **s =  (Student**)lua_newuserdata(L, sizeof(Student*));  // lua will manage Student** pointer
    *s = new Student;
    luaL_getmetatable(L, "cc.Student");
    lua_setmetatable(L, -2);
    return 1;
}


static void RegisterStaticCFun(lua_State * L, const char* gname, const char* funcName, lua_CFunction function) 
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
    lua_pushcclosure(L, function, 0);
    lua_settable(L, -3);
    
    lua_pop(L, 1); //gname table���ջ
}

//static const struct luaL_Reg studentlib_m [] = {
//    {"setName",l_setName},
//    {"print", l_print},
//    //{"__gc", auto_gc},
//    {NULL, NULL}
//};



int luaopen_student (lua_State *L) {
   
    RegisterStaticCFun(L,"cc","NEW",newStudent);

 
    luaL_newmetatable(L, "cc.Student");
    //lua_pushvalue(L, -1);

    lua_newtable(L);
    lua_pushstring(L, "setName");
    lua_pushcclosure(L, &l_setName, 0);
    lua_settable(L, -3); //���ڴ����ı���

    lua_pushstring(L, "print");
    lua_pushcclosure(L, &l_print, 0);
    lua_settable(L, -3); //���ڴ����ı���



    lua_setfield(L, -2, "__index");//���ϱߵ� table ���õ�Ԫ��  __index
    

    lua_pushcclosure(L, &l_gc, 0);
    lua_setfield(L, -2, "__gc");//����Ԫ��  __gc
    

    //luaL_setfuncs(L, studentlib_m, 0);//5.3
    //luaL_newlib(L, stuentlib_f);//5.2
    return 1;
}


int main_bind01()
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
    std::string sf = dir_scripts + "/test006.lua";
	if (luaL_loadfile(L, sf.c_str()))
	{
		cout << "Lua �ļ�����ʧ��" << endl;
		cout << lua_tostring(L, -1) << endl;
        return -2;
    }
	cout << "Lua �ļ�����["<< sf <<"] �ɹ�" << endl;
    
	//ִ�нű����غ�,Ĭ�ϵıհ�����,��ʼ����ǰ�ű�����
	if (lua_pcall(L, 0, 0, 0))
    {
		cout << "�ű���ʼ��ʧ��" << endl;
        cout << lua_tostring(L, -1) << endl;
        return -3;
    }

	luaopen_student(L);
	
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

	system("pause");
	return 0;
}





