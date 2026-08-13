#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(0);
    int n,m;
    std::cin >> n >> m;
    int i = 0;
    while (m > 0) {
        m = n%m;
        ++i;
    }
    std::cout << i << std::endl;
    return 0;
}