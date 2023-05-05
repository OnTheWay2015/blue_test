
extern "C" { // lua.dll　是c库，使用时要用 extern "C",  如果 lua库按 c++　编译的就用  __declspec(dllimport)/dllexport 这套

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
    lua_getglobal(L, gname);//gname table表压栈
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
    
    lua_pop(L, 1); //gname table表出栈
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
    lua_settable(L, -3); //放在创建的表上

    lua_pushstring(L, "print");
    lua_pushcclosure(L, &l_print, 0);
    lua_settable(L, -3); //放在创建的表上



    lua_setfield(L, -2, "__index");//把上边的 table 设置到元表  __index
    

    lua_pushcclosure(L, &l_gc, 0);
    lua_setfield(L, -2, "__gc");//设置元表  __gc
    

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



	///< 加载相关库文件
	luaL_openlibs(L);
    

    ///< 加载lua文件
    std::string sf = dir_scripts + "/test006.lua";
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

	luaopen_student(L);
	
	///指定 lua 入口函数 run 
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

	system("pause");
	return 0;
}





