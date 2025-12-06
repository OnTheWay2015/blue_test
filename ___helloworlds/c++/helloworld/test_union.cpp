#include "stdafx.h"
// 联合体与位域结合使用
#pragma pack(1) // 编译器默认字节对齐方式大多是4字节,这里指定 1 
union StatusUnion {
	struct {
		unsigned int vv1: 4;
		unsigned int vv2: 2;
		unsigned int vv3: 1;
		unsigned int vv4: 1;
	} bits;
	//uint16_t raw_value;
	uint8_t raw_value;
};
#pragma pack() //解除指定对齐



struct uuu
{
	unsigned short a;	//栈位置低
	unsigned short b;	//栈位置高
};
union MyUnion
{
	uuu v1;
	long v2;
};

//windows 是小端. 小端字节序是指数据的‌低位字节存储在低地址‌，高位字节存储在高地址
void test_union()
{
	MyUnion u;
	u.v2 = 0x12345678;

	printf("uuu.a[0x%hx] uuu.b[0x%hx]\n",u.v1.a,u.v1.b);

	/*
	printf 
		%hd   short
		%hu   unsigned short
		%hx	  short 16进制
	*/


	int sz = sizeof(StatusUnion);
	int sz1 = sizeof(StatusUnion::bits);
	int sz2 = sizeof(StatusUnion::raw_value);

	StatusUnion ss;
	ss.raw_value = 0xFF; //raw_value 是 unit8_t 只接收一个字节.  1111 1111 

	printf("vv1[0x%x] vv2[0x%x] vv3[0x%x] vv4[0x%x]\n",
		ss.bits.vv1,  //4 位,值为 0xf
		ss.bits.vv2,  //2 位,值为  0x3
		ss.bits.vv3,	//1 位,值为  0x1
		ss.bits.vv4		//1 位,值为  0x1
	);


}
