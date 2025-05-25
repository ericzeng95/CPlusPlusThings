#include "apple.h"
#include <iostream>

const int Apple::apple_number = 10;
int Apple::ap = 666;
Apple::Apple(int i) {}
int Apple::add(int num) {
  take(num);
  ap = 2;
  Apple::ap = 100;
  std::cout << "changed static to " << ap << std::endl;
  return 0;
}
int Apple::add(int num) const {
  take(num);
  Apple::ap = 900;
  std::cout << "changed static to " << ap << std::endl;
  return 0;
}
void Apple::take(int num) const {
  std::cout << "take func " << num << std::endl;
}
int Apple::getCount() const {
  take(1);
  //    add(); //error
  return apple_number;
}