#include <iostream>

int gcd(int a, int b) {
    return b == 0 ? a : gcd(b, a%b);
}

const int MAX = 100;
int a[MAX];

void solve() {
    int n;
    std::cin >> n;
    for (int i=0;i<n;++i) std::cin >> a[i];
    std::cout << gcd(a[0], a[n-1]) << std::endl;
}

int main(int argc, char** argv) {
#ifdef UT
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
    std::ios_base::sync_with_stdio(0);
    int t;
    std::cin >> t;
    while (t--) solve();
    return 0;
}
