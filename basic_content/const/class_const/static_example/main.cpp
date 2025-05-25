#include "apple.h"
#include <iostream>
int main() {
  Apple a(2);
  std::cout << a.getCount() << std::endl;
  a.add(10);
  std::cout << a.ap << std::endl;
  const Apple b(3);
  b.add(100);
  std::cout << a.ap << std::endl;
  return 0;
}
