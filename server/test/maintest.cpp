#include <iostream>
#include <map>
#include <windows.h>




extern void test_class_main();
extern void async_test();

struct xx {
    xx* pre;
    xx* next;
    int value;
};

int main()
{

    //std::map<int,int> mm;
    //mm[1] = 0x123;
    //mm[100] = 0x123456;

    xx* x = new xx();
    x->value = 0x123456;
    int n = 6; 
    xx* xptr =x;
    while(n>0)
    {
        xx* add = new xx();
        add->value = n;
        xptr->next = add;
        xptr = add;
        n--;
    }

    while(true)
    {
        Sleep(1);
    }

    //async_test();
    //test_class_main();

    //getchar();
    return 0;
}
