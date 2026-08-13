#include <iostream>

int main() {
  std::ios_base::sync_with_stdio(0);
  std::string role;
  std::cin >> role;
  if (role == "Algosia") {
    int a, b;
    std::cin >> a >> b;
    int diff = (a - b + 1000) % 1000;
    if (diff == 1 || diff == 999) {
      std::cout << (a + 2 - 1) % 1000 + 1 << " " << (b + 2 - 1) % 1000 + 1
                << std::endl;
    } else {
      std::cout << (a + 1 - 1) % 1000 + 1 << " " << (b + 1 - 1) % 1000 + 1
                << std::endl;
    }
  } else if (role == "Bajtek") {
    int a, b;
    std::cin >> a >> b;
    int diff = (a - b + 1000) % 1000;
    if (diff == 1 || diff == 999) {
      std::cout << (a + 1000 - 2 - 1) % 1000 + 1 << " "
                << (b + 1000 - 2 - 1) % 1000 + 1 << std::endl;
    } else {
      std::cout << (a + 1000 - 1 - 1) % 1000 + 1 << " "
                << (b + 1000 - 1 - 1) % 1000 + 1 << std::endl;
    }
  } else {
    std::cerr << "unsupported role" << std::endl;
  }
  return 0;
}