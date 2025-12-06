#include "stdafx.h"
#include  <functional>
#include <stdio.h>

class TEST
{
public:
	TEST():value(0) {};
	~TEST() {};
public:
	int value;
public:
	void act()
	{
		printf("stl_function TEST act\n");
	}
	int act1(int a,int b)
	{
		printf("stl_function TEST act1  res[%d]\n",a+b+value);
		return 0;
	}
	std::function<int(int,int)> fun1()
	{
		//直接返回 lambda 不用 std::bind，可不用 placeholders
		//return std::bind([this](int a,int b)->int{ return this->act1(a,b); },std::placeholders::_1, std::placeholders::_2);
		return [this](int a,int b)->int{ return this->act1(a,b); };
	}
	std::function<void()> fun()
	{
		return std::bind([this] { this->act(); });
	}
};

void test_lambda()
{
	int c,d = 0;
	//捕获列表 [],[=],[&],[=,&c],[&,c] [c,&d]
	auto lam = [&,c](int a, int b )->int { return a + b; };  
	auto lam1 = [](int a, int b )->int { return a + b; };
	auto lam2 = [](int a, int b ) { return a + b; }; //简单返回值,一般可简化
	auto lam3 = [=]{ return c + d; }; //无调用参数时可简化


}

void test_function()
{
	TEST t;
	auto f = std::bind([]{ return 1 + 2; });
	std::function<int()> f1 = std::bind([] { return 1 + 2; });

	auto f2 = t.fun1();
	f2(1,2);
}


void test_memberPtr()
{
	TEST t;
	int TEST::* valuep = &TEST::value; //成员指针
	printf("memberPtr value[%d] \n",t.*valuep);

	//c 方法指针
	void (*funcp)() = &test_lambda;
	(*funcp)();
	
	//auto TEST::* act = &TEST::act; //成员指针
	void (TEST::* act)() = &TEST::act; //成员指针
	(t.*act)();

	int (TEST::* act1)(int, int) = &TEST::act1; //成员指针
	(t.*act1)(11,22);
}


template<class T>
void fff(T a)
{
	printf(" fff(T) act \n");
}

template<>
void fff(int a) //特例化
{
	printf(" fff(int) act \n");
}

template<>
void fff(int* a) //特例化
{
	printf(" fff(int*) act \n");
}



template<typename First, typename ...Others> // 拆分首参数+剩余参数包
void fff(First first, Others... rest) {
	fff(first);          // 处理第一个参数（调用单参数版本）
	//if constexpr (sizeof...(rest) > 0) { // C++17 constexpr if（C++11可省略，直接递归）
		fff(rest...);    // 递归处理剩余参数（直到只剩一个参数）
	//}
}


void test_functional()
{
	int arr[]{1,3,4};
	fff("1", "2", 3, 4, 5, 6,arr);
	test_lambda();
	test_function();
	test_memberPtr();
}




