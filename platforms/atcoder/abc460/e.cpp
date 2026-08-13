#include <iostream>

const int64_t M = 998244353l;

__int128_t gcd(__int128_t a, __int128_t b) {
  return b == 0 ? a : gcd(b, a % b);
}

void solve() {
  int64_t n, m;
  std::cin >> n >> m;

  int64_t sum = 0;
  __int128_t d = 10;
  while (d / 10 <= n) {
    __int128_t dx = std::min(d, __int128_t(n + 1));
    // how many x such x*d=x (mod m)
    // x(d-1) = 0 (mod m)
    // x = 0 (mod m/g)
    auto g = gcd(d-1, m);
    sum += (dx-d/10) * (n / (m / g)) % M;
    d *= 10;
  }
  std::cout << sum % M << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(0);
  int t;
  std::cin >> t;
  while (t--) solve();
  return 0;
}
