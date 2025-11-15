#include "stdafx.h"

// 重写全局operator new
void* operator new(std::size_t size) {
    void* ptr = malloc(size);
    printf("Memory allocated at {%p}: size={%d}\n", ptr, size);
    return ptr;
}

// 重写全局operator delete
void operator delete(void* ptr) noexcept {
    printf("Memory freed at {%p}\n", ptr);
    free(ptr);
}


class xxxx
{
public:
    xxxx(std::string x) {
        printf("xxxx() act \n");
    }
public:
    int a;

};

//宏定义里的 #,## 需要在预编译时期可以计算的.
//#  获取 变量名 为 字符串
//## 字符串 连接运算
#include <utility>
#define xNEW(tp, ...) \
[&]() -> tp* { \
    printf("xNEW %s  act \n",#tp###tp##"gogogo");\
    void* ptr = malloc(sizeof(tp)); \
    if (ptr == nullptr) { \
        std::cerr << "Memory allocation failed for type: " #tp << std::endl; \
        return nullptr; \
    } \
    try { \
        return new (ptr) tp(__VA_ARGS__); \
    } catch (...) { \
        free(ptr); \
        std::cerr << "Constructor failed for type: " #tp << std::endl; \
        return nullptr; \
    } \
}()



void test_new_memory()
{

    //auto p = new xxxx("666");
    auto px = xNEW(xxxx,"999");
    int x = 123;
}





