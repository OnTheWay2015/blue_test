#include "stdafx.h"

#include <set>


template <typename T>
class Ptr {
 public:
  Ptr() : p_(NULL) {
  }

  // This constructor shall not be explicit.
  // lint does not like this but this is how it works.
  Ptr(T* pT) : p_(NULL) {
    *this = pT;
  }

  Ptr(const Ptr<T>& p) : p_(NULL) {
    *this = p;
  }

  ~Ptr() {
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

//std::set �����Զ���ȽϺ��� test_set_compare_fun  
//�ȽϺ�������д�ɷº�����class��struct��lambda��
//������д�ɺ���

typedef std::set<HeaderPtr , test_set_compare_fun> HeaderOffsetSortedSet;
void test_set()
{
    HeaderOffsetSortedSet records;
    auto table = new test_set_a();
    records.insert(table);
}

