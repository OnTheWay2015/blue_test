#include "stdafx.h"


extern void test_set();
extern void test_new_memory();
extern void test_sort();


void testAddress() 
{
    struct xst {
    public:
        int str[10];
    };

    xst ary[3];
   

    for (int i = 0; i < 3;i++ ) {
        memset((ary[i].str), 0, 10);
        //ary[i].str[0] = 1;
    }

    for (int i = 0; i < 3;i++ ) {
        printf("str int:%d\n",ary[i].str[0]);
    }

    auto x = *(xst*)ary;
    auto xx = &ary;

    auto xxx = &x;

    auto xxxx = (xst*)ary + 1;
    xxxx->str[0] = 666;

    //地址值 和指针，引用 是同一个东西，只是语法糖包装不一样

    unsigned long long aryP1 = (unsigned long)(&ary[0]); //取数组第一个元素的地址, 转为地址数值
    //aryP1 和  aryP  值是一样的
    unsigned long long aryP = (unsigned long)ary; //数组名是一个指针，直接可以转为地址数值
    int s_xst = sizeof(xst);
    auto xxxxx = (xst*)( (unsigned long long)ary + sizeof(xst)*2); //偏移两个数组元素，在第三个修改
    xxxxx->str[0] = 999;
}






extern void test_union();
extern void stl_vector();
extern void test_functional();

int main()
{
    test_functional();
   //stl_vector();

    //test_union();
    //test_sort();
    //test_new_memory();
    //testAddress();
    //test_set();
    return 0;
}


