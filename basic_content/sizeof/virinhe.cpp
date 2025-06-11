/**
 * @file virnhe.cpp
 * @brief 虚继承
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include <iostream>

using namespace std;

class A {
  virtual void fun() {}
};

class B {
  virtual void fun2() {}
  virtual void fun3() {}
};

class D {
  virtual void fun8() {}
  virtual void fun9() {}
};

class C : virtual public A, virtual public B, virtual public D {
public:
  virtual void fun3() {}
  virtual void fun4() {}
};

int main() {

  /**
   * @brief 8 8 16  派生类虚继承多个虚函数，会继承所有虚函数的vptr
   */
  cout << sizeof(A) << " " << sizeof(B) << " " << sizeof(C);

  return 0;
}
