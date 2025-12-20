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
void test_template_args(T a)
{
	printf(" test_template_args(T) act \n");
}

template<>
void test_template_args(int a) //特例化
{
	printf(" test_template_args(int) act \n");
}

template<>
void test_template_args(int* a) //特例化
{
	printf(" test_template_args(int*) act \n");
}

void test_template_args() //特例化
{
	printf(" test_template_args()  END!! \n");
}

//模板可变参数
template<typename First, typename ...Others> // 拆分首参数+剩余参数包
void test_template_args(First first, Others... rest) {
	test_template_args(first);          // 处理第一个参数（调用单参数版本）
	//if constexpr (sizeof...(rest) > 0) { // C++17 constexpr if（C++11可省略，直接递归）
		test_template_args(rest...);    // 递归处理剩余参数（直到只剩一个参数）
	//}
}

#include <stdarg.h>
//可变参数
int test_args(int argscnt, ...)
{
	int i, result = 0;
	va_list vl;     //va_list指针，参数起始地址, 用于va_start取可变参数，为char*
	
	// 初始化可变参数列表 vl. argscnt 也可指定具体数值(一般不这样用,此时参数个数是固定的)
	// 计算函数第一个参数在栈里相对起始参数的偏移量
	va_start(vl, argscnt);       
	printf("argscnt:%d, vl:%d\n", argscnt, *vl); //v1此时指向第一个参数, argscnt 后面第一个 
	
	//当前第一个参数为 有效参数的数量
	for (i = 0; i < argscnt; i++)//这里 argscnt 表示可变参数列表中有多少个参数  
	{
		//取当前 v1 地值的值，返回为 result.	
		result = va_arg(vl, int);//这里把vl往后跳过4个字节（sizeof(int)大小）指向下一个参数，返回的是当前参数（而非下一个参数）  
		printf("in for  result:%d,  *vl:%d\n", result, *vl);//这里打印下，可以看出，vl总是指向result后面的那个参数  
	}
	va_end(vl);//结束标志  

	return result;

}


void test_functional()
{
	int arr[]{1,3,4};
	
	test_args(5,1,2,3,4,5);
	test_template_args("1", "2", 3, 4, 5, 6,arr);
	test_lambda();
	test_function();
	test_memberPtr();
}




