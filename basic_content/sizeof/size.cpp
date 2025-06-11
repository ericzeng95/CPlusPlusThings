/**
 * @file moreinhe.cpp
 * @brief 普通多继承与虚函数多继承
 * @author 光城
 * @version v1
 * @date 2019-07-21
 */

#include <cstddef>
using namespace std;

template <typename T, size_t N>
constexpr int getSize(T (&v)[N]) 
{ return N; }

int main() {
  std::byte bytes[15];
  char chars[20];

  static_assert(getSize(bytes) == 15);
  static_assert(getSize(chars) == 20);
}
