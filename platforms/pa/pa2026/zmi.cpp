#include <iostream>

long to_m(long d, long h, long m) {
    return (d*24+h)*60+m;
}

long diff(long end, long start) {
    long change = to_m(29, 2, 0);
    if (end > change & start <= change) return end-start-60;
    else return end-start;
}

long R[6] = {
    to_m(0, 0, 0),
    to_m(25, 0, 0),
    to_m(26, 0, 0),
    to_m(27, 0, 0),
    to_m(28, 0, 0),
    to_m(30, 0, 0)
};

int main() {
    std::ios_base::sync_with_stdio(0);
    long x,d,h,m;
    std::cin >> x >> d >> h >> m;

    std::cout << diff(R[x], to_m(d,h,m)) << std::endl;
    return 0;
}
