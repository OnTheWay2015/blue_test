#include "stdafx.h"

class c1
{
    public :
virtual void f1()=0;
virtual void f3()=0;
};

class c2
{
    public :
virtual void f2()=0;
};

class c3
{
    public :
virtual void f3()=0;
};

class cc :public c1, public c2
{
    public :
virtual void f1() override {};
virtual void f2() override {};
virtual void f3() override {};
};


class cccc 
{
    void f3() {};
};



void test_class_main()
{
}



