#include <iostream>
using namespace std;

enum class Color { RED, BLUE };
enum class Feeling { EXCITED, BLUE };

int main() {
  Color a = Color::BLUE;
  Feeling b = Feeling::EXCITED;
  std::cout << static_cast<int>(a) << ":" << static_cast<int>(b) << std::endl;
  return 0;
}
