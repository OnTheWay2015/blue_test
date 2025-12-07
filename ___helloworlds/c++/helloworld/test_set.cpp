#include "stdafx.h"

#include <set>


template <typename T>
class Ptr {
 public:
  Ptr() : p_(NULL) {
  }

  // This constructor shall not be explicit.
  // lint does not like this but this is how it works.
/*
Ptr(T* pT) 构造函数中执行 *this = pT → 尝试调用 operator=(T*)；
由于未自定义 operator=(T*)，编译器会将 T* 隐式转换为 Ptr<T>（因为 Ptr(T*) 不是 explicit）；
转换过程会再次调用 Ptr(T*) 构造函数，新构造的临时 Ptr 对象又会执行 *this = pT，形成无限递归；
递归无终止条件，最终触发堆栈溢出警告（运行时会直接崩溃）。
*/
  Ptr(T* pT) : p_(NULL) {
    *this = pT; // -->  Ptr(const Ptr<T>& p) 
  }

  Ptr(const Ptr<T>& p) : p_(NULL) {
    *this = p;
  }

  ~Ptr() {
  }
  // ========== 核心修复：显式定义赋值运算符 ==========
  // 1. 裸指针赋值运算符（避免隐式转换为Ptr<T>）
  Ptr<T>& operator=(T* pT) {
      if (p_ != pT) { // 避免自赋值
          // 示例：若Ptr负责内存释放，先释放旧指针
          // delete p_;
          p_ = pT; // 直接赋值裸指针，无递归
      }
      return *this; // 返回自身，支持链式赋值
  }

  // 2. Ptr<T>拷贝赋值运算符
  Ptr<T>& operator=(const Ptr<T>& p) {
      if (this != &p) { // 避免自赋值
          // 示例：若Ptr负责内存释放，先释放旧指针
          // delete p_;
          p_ = p.p_; // 拷贝指针（浅拷贝，若需深拷贝需自定义）
      }
      return *this;
  }


  mutable T* p_;
};


class test_set_a {
   public:
       int a; 
};
typedef Ptr<test_set_a> HeaderPtr;

class test_set_compare_fun{
   public:
       int a; 
    bool operator()(const HeaderPtr a,const HeaderPtr b) const {
        return false;
    } 
};

//std::set 设置自定义比较函数 test_set_compare_fun  
//比较函数必须写成仿函数（class、struct、lambda）
//而不能写成函数

typedef std::set<HeaderPtr , test_set_compare_fun> HeaderOffsetSortedSet;
void test_set()
{
    HeaderOffsetSortedSet records;
    auto table = new test_set_a();
    records.insert(table);
}

