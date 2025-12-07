
#include "stdafx.h"
#include <iostream>
#include <type_traits>
#include <cmath>

// ===================== 场景1：基础编译期算术运算（模板非类型参数） =====================
// 模板实现编译期加法
template <int A, int B>
struct Add {
    static constexpr int value = A + B; // 编译期常量
};

// 模板实现编译期减法
template <int A, int B>
struct Sub {
    static constexpr int value = A - B;
};

// 模板实现编译期乘法
template <int A, int B>
struct Mul {
    static constexpr int value = A * B;
};

// 模板实现编译期除法（处理除数为0的编译期断言）
template <int A, int B>
struct Div {
    static_assert(B != 0, "Divisor cannot be zero!"); // 编译期检查
    static constexpr int value = A / B;
};

// ===================== 场景2：编译期递归运算（阶乘/斐波那契） =====================
// 编译期阶乘（递归模板）
template <int N>
struct Factorial {
    static_assert(N >= 0, "Factorial only supports non-negative integers!");
    static constexpr int value = N * Factorial<N - 1>::value; // 递归展开
};

// 阶乘终止条件（模板特化）
template <>
struct Factorial<0> {
    static constexpr int value = 1; // 0! = 1
};

// 编译期斐波那契数列（递归模板）
template <int N>
struct Fibonacci {
    static_assert(N >= 0, "Fibonacci only supports non-negative integers!");
    static constexpr int value = Fibonacci<N - 1>::value + Fibonacci<N - 2>::value;
};

// 斐波那契终止条件1
template <>
struct Fibonacci<0> {
    static constexpr int value = 0;
};

// 斐波那契终止条件2
template <>
struct Fibonacci<1> {
    static constexpr int value = 1;
};

// ===================== 场景3：类型推导的运算（基于类型的数值运算） =====================
// 模板推导类型，计算类型大小的倍数
template <typename T, int N>
struct TypeSizeMul {
    static constexpr size_t value = sizeof(T) * N; // 编译期计算类型大小*N
};

// ===================== 场景4：条件运算（编译期分支） =====================
// 编译期绝对值（if constexpr C++17+）
//template <int N>
//struct Abs {
//    static constexpr int value = []() constexpr {
//        if constexpr (N >= 0) {
//            return N;
//        }
//        else {
//            return -N;
//        }
//        }(); // 编译期lambda
//};

// 编译期最大值（模板特化实现）
template <int A, int B>
struct Max {
    static constexpr int value = (A > B) ? A : B;
};

// ===================== 场景5：自定义运算规则（模板参数化运算） =====================
// 通用运算模板（参数化运算规则）
template <int A, int B, template<int, int> class Op>
struct Calculate {
    static constexpr int value = Op<A, B>::value; // 应用自定义运算
};

// 自定义运算：取模
template <int A, int B>
struct Mod {
    static_assert(B != 0, "Mod divisor cannot be zero!");
    static constexpr int value = A % B;
};

// 自定义运算：平方和
template <int A, int B>
struct SumOfSquare {
    static constexpr int value = A * A + B * B;
};

// ===================== 场景6：浮点数编译期运算（C++20 constexpr） =====================
// C++20支持浮点数编译期运算
#if __cpp_constexpr >= 201907L
template <double A, double B>
struct FloatAdd {
    static constexpr double value = A + B;
};

template <double A>
struct Sqrt {
    static constexpr double value = std::sqrt(A); // C++20 std::sqrt支持constexpr
};
#endif

// ===================== 测试代码 =====================
int test_templatecalc() {
    // 场景1：基础算术运算
    std::cout << "===== 基础编译期算术运算 =====" << std::endl;
    std::cout << "3 + 5 = " << Add<3, 5>::value << std::endl;
    std::cout << "10 - 4 = " << Sub<10, 4>::value << std::endl;
    std::cout << "6 * 7 = " << Mul<6, 7>::value << std::endl;
    std::cout << "20 / 4 = " << Div<20, 4>::value << std::endl;
    // std::cout << Div<5, 0>::value << std::endl; // 编译报错：除数为0

    // 场景2：递归运算
    std::cout << "\n===== 编译期递归运算 =====" << std::endl;
    std::cout << "5! = " << Factorial<5>::value << std::endl; // 5*4*3*2*1=120
    std::cout << "Fib(10) = " << Fibonacci<10>::value << std::endl; // 55

    // 场景3：类型推导运算
    std::cout << "\n===== 类型推导运算 =====" << std::endl;
    std::cout << "sizeof(int) * 3 = " << TypeSizeMul<int, 3>::value << std::endl; // 4*3=12
    std::cout << "sizeof(double) * 2 = " << TypeSizeMul<double, 2>::value << std::endl; // 8*2=16

    // 场景4：条件运算
    std::cout << "\n===== 编译期条件运算 =====" << std::endl;
    //std::cout << "Abs(-8) = " << Abs<-8>::value << std::endl; // 8
    std::cout << "Max(15, 9) = " << Max<15, 9>::value << std::endl; // 15

    // 场景5：自定义运算规则
    std::cout << "\n===== 自定义运算规则 =====" << std::endl;
    // 应用Mod运算
    std::cout << "17 % 5 = " << Calculate<17, 5, Mod>::value << std::endl; // 2
    // 应用平方和运算
    std::cout << "3? + 4? = " << Calculate<3, 4, SumOfSquare>::value << std::endl; // 25

    // 场景6：浮点数编译期运算（C++20）
#if __cpp_constexpr >= 201907L
    std::cout << "\n===== 浮点数编译期运算（C++20） =====" << std::endl;
    std::cout << "3.5 + 2.8 = " << FloatAdd<3.5, 2.8>::value << std::endl; // 6.3
    std::cout << "sqrt(16.0) = " << Sqrt<16.0>::value << std::endl; // 4.0
#endif

    return 0;
}