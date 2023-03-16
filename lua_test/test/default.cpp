#include <iostream>

extern "C" { // lua.dll����c�⣬ʹ��ʱҪ�� extern "C",  ��� lua�ⰴ c++������ľ���  __declspec(dllimport)/dllexport ����

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

using namespace std;


/*
//ջ����
--[[   p_func_proto(Proto *p) ���
 Proto: --------------------

main <@../../test/scripts/test001.lua:0,0> (18 instructions, 72 bytes at 000001A8325A1ED0)
0+ params, 3 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 0 locals, 12 constants, 2 functions
function Instructions start:
        1       line[1] LOADK                   ; "I'm lua"
        2       line[1] SETGLOBAL               ; mystr
        3       line[3] NEWTABLE        iABC argA[0]
        4       line[3] SETTABLE        iABC argA[0]    ; "name" "xiaomign"
        5       line[3] SETTABLE        iABC argA[0]    ; "id" 123456
        6       line[3] SETGLOBAL               ; myTable
        7       line[4] NEWTABLE        iABC argA[0]
        8       line[4] LOADK                   ; "a1"
        9       line[4] LOADK                   ; "a2"
        10      line[4] SETLIST         iABC argA[0]    ; 1
        11      line[4] SETGLOBAL               ; myAry
        12      line[8] CLOSURE                 ; 000001A832594A60
        13      line[6] SETGLOBAL               ; print_hello
        14      line[12]        CLOSURE                 ; 000001A832594B20
        15      line[10]        SETGLOBAL               ; _add
        16      line[14]        GETGLOBAL               ; print_hello
        17      line[14]        CALL            iABC argA[0]
        18      line[14]        RETURN          iABC argA[0]
function Instructions end!
constants (12) for 000001A8325A1ED0:
        1       "mystr"
        2       "I'm lua"
        3       "myTable"
        4       "name"
        5       "xiaomign"
        6       "id"
        7       123456
        8       "myAry"
        9       "a1"
        10      "a2"
        11      "print_hello"
        12      "_add"
locals (0) for 000001A8325A1ED0:
upvalues (0) for 000001A8325A1ED0:

function <@../../test/scripts/test001.lua:6,8> (4 instructions, 16 bytes at 000001A832594A60)
0 params, 2 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 0 locals, 2 constants, 0 functions
function Instructions start:
        1       line[7] GETGLOBAL               ; print
        2       line[7] LOADK                   ; "hello lua"
        3       line[7] CALL            iABC argA[0]
        4       line[8] RETURN          iABC argA[0]
function Instructions end!
constants (2) for 000001A832594A60:
        1       "print"
        2       "hello lua"
locals (0) for 000001A832594A60:
upvalues (0) for 000001A832594A60:

function <@../../test/scripts/test001.lua:10,12> (3 instructions, 12 bytes at 000001A832594B20)
2 params, 3 slots(maxstacksize), 0 upvalues, sizeupvalues[0] 2 locals, 0 constants, 0 functions
function Instructions start:
        1       line[11]        ADD             iABC argA[2]
        2       line[11]        RETURN          iABC argA[2]
        3       line[12]        RETURN          iABC argA[0]
function Instructions end!
constants (0) for 000001A832594B20:
locals (2) for 000001A832594B20:
        0       a       1       3
        1       b       1       3
upvalues (0) for 000001A832594B20:


--]]


*/



void test_default()
{
    //	cout << "top:"<< L->top - L->base  << endl;  ��ǰ����֡��,ջ��ʹ�õĿռ��С


}

//metatable Ԫ��
/*
* 
*
��C����ʵ��һ��Lua�Ĳ������飬���ṩ�����ִ��Ч��

#include <lua.hpp>
#include <lauxlib.h>
#include <lualib.h>
#include <limits.h>
#define BITS_PER_WORD (CHAR_BIT * sizeof(int))
#define I_WORD(i)     ((unsigned int)(i))/BITS_PER_WORD
#define I_BIT(i)      (1 << ((unsigned int)(i)%BITS_PER_WORD))

typedef struct NumArray {
    int size;
    unsigned int values[1];
} NumArray;

extern "C" int newArray(lua_State* L)
{
    //1. ����һ�������Ƿ�Ϊ���͡��Լ��ò�����ֵ�Ƿ���ڵ���1.
    int n = luaL_checkint(L,1);
    luaL_argcheck(L, n >= 1, 1, "invalid size.");
    size_t nbytes = sizeof(NumArray) + I_WORD(n - 1) * sizeof(int);
    //2. ������ʾLuaΪuserdata������ֽ�����ͬʱ��������userdata����ѹ��ջ�С�
    NumArray* a = (NumArray*)lua_newuserdata(L,nbytes);
    a->size = n;
    for (int i = 0; i < I_WORD(n - 1); ++i)
        a->values[i] = 0;
    //��ȡע������myarray����key��ֵΪmetatable��
    luaL_getmetatable(L,"myarray");
    //��userdata��Ԫ������Ϊ��myarray������table��ͬʱ��ջ��Ԫ�ص�����
    lua_setmetatable(L,-2);
    return 1;
}

extern "C" int setArray(lua_State* L)
{
    //1. Lua�����ú����ĵ�һ������������userdata���ö����Ԫ��Ҳ������ע����к�myarray������table��
    //����ú���������ֹ����
    NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
    int index = luaL_checkint(L,2) - 1;
    //2. �����κ����͵����ݶ����Գ�Ϊ����ֵ���������ʹ��anyֻ��Ϊ��ȷ����3��������
    luaL_checkany(L,3);
    luaL_argcheck(L,a != NULL,1,"'array' expected.");
    luaL_argcheck(L,0 <= index && index < a->size,2,"index out of range.");
    if (lua_toboolean(L,3))
        a->values[I_WORD(index)] |= I_BIT(index);
    else
        a->values[I_WORD(index)] &= ~I_BIT(index);
    return 0;
}

extern "C" int getArray(lua_State* L)
{
    NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
    int index = luaL_checkint(L,2) - 1;
    luaL_argcheck(L, a != NULL, 1, "'array' expected.");
    luaL_argcheck(L, 0 <= index && index < a->size,2,"index out of range");
    lua_pushboolean(L,a->values[I_WORD(index)] & I_BIT(index));
    return 1;
}

extern "C" int getSize(lua_State* L)
{
    NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
    luaL_argcheck(L,a != NULL,1,"'array' expected.");
    lua_pushinteger(L,a->size);
    return 1;
}

extern "C" int array2string(lua_State* L)
{
    NumArray* a = (NumArray*)luaL_checkudata(L,1,"myarray");
    lua_pushfstring(L,"array(%d)",a->size);
    return 1;
}

static luaL_Reg arraylib_f [] = {
    {"new", newArray},
    {NULL, NULL}
};

static luaL_Reg arraylib_m [] = {
    {"set", setArray},
    {"get", getArray},
    {"size", getSize},
    {"__tostring", array2string}, //print(a)ʱLua����ø�Ԫ������
    {NULL, NULL}
};

extern "C" __declspec(dllexport)
int luaopen_testuserdata(lua_State* L)
{
    //1. ����Ԫ��������Ԫ��ָ����newArray�����´�����userdata����Lua��userdataҲ����table����ݱ��ֵġ�
    //�����ڵ��ö�����ʱ������ͨ����֤��metatable��������ȷ������userdata�Ƿ�Ϸ���
    luaL_newmetatable(L,"myarray");
    lua_pushvalue(L,-1);
    //2. Ϊ��ʵ����Զ���ĵ��÷�ʽ����Ҫ��Ԫ���__index�ֶ�ָ������ͬʱ�ٽ�arraylib_m�����еĺ���ע�ᵽ
    //Ԫ���У�֮�������Щע�ắ���ĵ��þͿ���������������ʽ�����ˡ�
    //lua_setfield��ִ�к�Ὣջ����table������
    lua_setfield(L,-2,"__index");
    //����Щ��Ա����ע���Ԫ���Ա�֤Lua��Ѱ�ҷ���ʱ���Զ�λ��NULL������ʾ����ջ����table����ڶ���������
    luaL_register(L,NULL,arraylib_m);
    //����ֻע��Ĺ���������
    luaL_register(L,"testuserdata",arraylib_f);
    return 1;
}


*/
int testMetatable(lua_State *L)
{
    luaL_newmetatable(L,"myarray"); //ѹջ+1

    //lua_setiuservalue();
    return 0;
}



//userdata
/*
userdata �� uservalue �Ĺ�ϵ�����ù�ϵ��Ҳ���� uservalue ������������ userdata ����������һ�¡�

int lua_getiuservalue (lua_State *L, int idx, int n) ����ȡ���� userdata �ϵ� uservalue��
int lua_setiuservalue (lua_State *L, int idx, int n) ������ userdata �ϵ� uservalue��

*/

int testUserData(lua_State *L)
{

    return 0;
}



int testDefault(lua_State *L)
{
    //testMetatable(L);
    testUserData(L);
    return 0;
}



