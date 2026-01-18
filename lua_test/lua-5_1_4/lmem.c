/*
** $Id: lmem.c,v 1.70.1.1 2007/12/27 13:02:25 roberto Exp $
** Interface to Memory Manager
** See Copyright Notice in lua.h
*/


#include <stddef.h>

#define lmem_c
#define LUA_CORE

#include "lua.h"

#include "ldebug.h"
#include "ldo.h"
#include "lmem.h"
#include "lobject.h"
#include "lstate.h"



/*
** About the realloc function:
** void * frealloc (void *ud, void *ptr, size_t osize, size_t nsize);
** (`osize' is the old size, `nsize' is the new size)
**
** Lua ensures that (ptr == NULL) iff (osize == 0).
**
** * frealloc(ud, NULL, 0, x) creates a new block of size `x'
**
** * frealloc(ud, p, x, 0) frees the block `p'
** (in this specific case, frealloc must return NULL).
** particularly, frealloc(ud, NULL, 0, 0) does nothing
** (which is equivalent to free(NULL) in ANSI C)
**
** frealloc returns NULL if it cannot create or reallocate the area
** (any reallocation to an equal or smaller size cannot fail!)
*/



#define MINSIZEARRAY	4


void *luaM_growaux_ (lua_State *L, void *block, int *size, size_t size_elems,
                     int limit, const char *errormsg) {
  void *newblock;
  int newsize;
  if (*size >= limit/2) {  /* cannot double it? */
    if (*size >= limit)  /* cannot grow even a little? */
      luaG_runerror(L, errormsg);
    newsize = limit;  /* still have at least one free place */
  }
  else {
    newsize = (*size)*2;
    if (newsize < MINSIZEARRAY)
      newsize = MINSIZEARRAY;  /* minimum size */
  }
  newblock = luaM_reallocv(L, block, *size, newsize, size_elems);
  *size = newsize;  /* update only when everything else is OK */
  return newblock;
}


void *luaM_toobig (lua_State *L) {
  luaG_runerror(L, "memory allocation error: block too big");
  return NULL;  /* to avoid warnings */
}



///*
//** generic allocation routine.
//*/
//void *luaM_realloc_ (lua_State *L, void *block, size_t osize, size_t nsize) {
//  global_State *g = G(L);
//  lua_assert((osize == 0) == (block == NULL));
//  block = (*g->frealloc)(g->ud, block, osize, nsize);
//  if (block == NULL && nsize > 0)
//    luaD_throw(L, LUA_ERRMEM);
//  lua_assert((nsize == 0) == (block == NULL));
//  g->totalbytes = (g->totalbytes - osize) + nsize;
//  return block;
//}

/**
 创建，释放都走这个接口

 * @brief Lua 虚拟机底层内存重分配核心函数
 *        负责内存的分配、扩容、缩容和释放，是 Lua 所有内存操作的统一入口
 * @param L Lua 线程状态指针，用于获取全局状态和抛出内存错误
 * @param block 待操作的内存块指针（已分配的内存块 / NULL）
 * @param osize 内存块的原始大小（单位：字节）
 * @param nsize 内存块的目标大小（单位：字节）
 * @return 成功：返回重新分配后的内存块指针（可能是新地址/原地址/NULL）；失败：触发内存错误异常，不返回
 */
void* luaM_realloc_(lua_State* L, void* block, size_t osize, size_t nsize) {
    // 1. 获取当前 Lua 线程对应的全局状态结构体（存储内存总量、内存分配器等全局信息）
    global_State* g = G(L);

    // 2. 断言校验：保证「原始大小为 0」和「内存块为 NULL」是等价的（Lua 内存管理的核心约束）
    //    - 若 osize=0，则 block 必须为 NULL（表示首次分配内存）
    //    - 若 block=NULL，则 osize 必须为 0（避免无效的内存块大小匹配）
    lua_assert((osize == 0) == (block == NULL));

    // 3. 调用全局状态中注册的内存分配器，执行实际的内存重分配操作
    //    - g->frealloc：自定义/默认的内存分配器函数指针（遵循 Lua 内存分配器规范）
    //    - g->ud：内存分配器的用户自定义数据（透传给 frealloc，用于上下文传递）
    //    - 该调用支持四种场景：分配（block=NULL、osize=0、nsize>0）、扩容/缩容（block≠NULL、nsize>0）、释放（block≠NULL、nsize=0）
    block = (*g->frealloc)(g->ud, block, osize, nsize); // --> l_alloc() 

    // 4. 内存分配失败处理：若目标大小>0但返回的内存块为NULL，说明内存分配/重分配失败
    //    - luaD_throw：触发 Lua 底层长跳转，抛出内存不足错误（LUA_ERRMEM）
    //    - 该错误会被 Lua 受保护执行环境捕获，不会直接崩溃虚拟机
    if (block == NULL && nsize > 0)
        luaD_throw(L, LUA_ERRMEM);

    // 5. 断言校验：保证「目标大小为 0」和「内存块为 NULL」是等价的（内存释放后的约束）
    //    - 若 nsize=0，则 block 必须为 NULL（表示内存已成功释放）
    //    - 若 block=NULL，则 nsize 必须为 0（避免释放后返回无效内存块）
    lua_assert((nsize == 0) == (block == NULL));

    // 6. 更新 Lua 全局内存总量：扣除原始内存大小，加上目标内存大小
    //    - g->totalbytes：记录 Lua 虚拟机当前管理的所有内存总量（用于 GC 阈值判断）
    //    - 该计算支持：分配内存（总量增加）、释放内存（总量减少）、扩容/缩容（总量增减差值）
    g->totalbytes = (g->totalbytes - osize) + nsize;

    // 7. 返回重新分配后的内存块指针（业务层可直接使用该指针操作内存）
    return block;
}
