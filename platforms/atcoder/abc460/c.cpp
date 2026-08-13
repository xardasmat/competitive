#include <iostream>
#include <algorithm>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(0);
    int n,m;
    std::cin >> n >> m;
    std::vector<int> A,B;
    for (int i=0;i<n;++i) {
        int a;
        std::cin >> a;
        A.push_back(a);
    }
    for (int i=0;i<m;++i) {
        int b;
        std::cin >> b;
        B.push_back(b);
    }
    std::sort(A.begin(), A.end());
    std::sort(B.begin(), B.end());

    int b = 0;
    int sum = 0;
    for (int a : A) {
        if (b < B.size() && B[b] <= 2*a) {
            ++sum;
            ++b; 
        }
    }
    std::cout << sum << std::endl;
    return 0;
}