#include <iostream>

long sqr(long x) {
    return x*x;
}

bool solve() {
    long x1,y1,r1,x2,y2,r2;
    std::cin >> x1 >> y1 >> r1 >> x2 >> y2 >> r2;
    long dist = sqr(x1-x2) + sqr(y1-y2);
    return dist <= sqr(r1+r2) and dist >= sqr(r1-r2);
}

int main() {
    std::ios_base::sync_with_stdio(0);
    int t;
    std::cin >> t;
    while (t--) std::cout << ( solve() ? "Yes" : "No") << std::endl;
    return 0;
}