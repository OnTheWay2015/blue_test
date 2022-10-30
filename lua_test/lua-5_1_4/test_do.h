/*************************************************************************
	> File Name: 3rd/lua-5_1_4/test_do.h
	> Author: blue
	> Mail:   
	> Created Time: Sun 23 Dec 2018 06:31:07 PM CST
 ************************************************************************/

#ifndef _TEST_DO_H
#define _TEST_DO_H

#include "lua.h"
#include "lstate.h"
#include "lobject.h"
#include "ldo.h"

//#define g_debug(fmt, args...) do{ printf("\033[1;31;40m][lua]PID[%d] FILE[%s] LINE[%d] ",getpid(), __FILE__, __LINE__); printf( fmt, ## args); printf("\n\033[0m]"); } while(0);
//extern struct CallInfo;

#endif // _TEST_DO_H



extern void p_callinfo(char* str, struct CallInfo *ci);
extern void p_lua_state(lua_State *L);
extern void p_lua_stack(lua_State *L, StkId st, StkId ed);

extern int print_stack(lua_State *L);


void p_func_proto(Proto* p);

void stackDump(lua_State* L);
