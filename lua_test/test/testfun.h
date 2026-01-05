#pragma once

extern "C" { 

#include "../lua-5_1_4/lua.h"
#include "../lua-5_1_4/lualib.h"
#include "../lua-5_1_4/lauxlib.h"
#include "../lua-5_1_4/test_do.h"

}

template<typename CALLFUN>
int _wrapfunc(lua_State* L, CALLFUN func)
{

	return 0;
}


typedef void(*FF)();
template<>
int _wrapfunc(lua_State* L, FF func)
{
	(*func)();
	return 0;
}



template<typename CALLFUN>
int wrapfunc(lua_State* L)
{
	void* d = lua_touserdata(L,lua_upvalueindex(1));

	auto x = d;
	auto func = dynamic_cast<CALLFUN>(*d); // error todo


	return 1;
}


template<typename CALL_FUN>
void _ToluaRegfuncs(lua_State* L, const char* name, CALL_FUN func)
{
	if (!lua_istable(L, -1))
	{
		//assert(true);
		return;
	}
	
	lua_pushstring(L,name);
	void* d = lua_newuserdata(L,sizeof(func));
	memcpy(d,&func,sizeof(func));

	lua_pushcclosure(L, [](lua_State* L) {
		return wrapfunc<CALL_FUN>(L);
		},1);
	
	lua_rawset(L,-3);


}


class baseitem
{
public:
	baseitem() {}
	~baseitem() {}
public:
	void ToluaMergefunc(lua_State* L);
	void ToluaRegfuncsEnd(lua_State* L);

protected:
	int getInt();
protected:
	virtual void ToluaRegfuncs(lua_State* L);
};
