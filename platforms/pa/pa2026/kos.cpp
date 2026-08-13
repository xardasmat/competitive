#include <iostream>

template <long MOD>
class int_mod {
 public:
  int_mod(long value = 0) {
    if (value < 0)
      value %= MOD;
    else if (value >= MOD)
      value %= MOD;
    if (value < 0) value += MOD;
    v = value;
  }

  static long pow(long a, long n) {
    long result = 1;
    while (n > 0) {
      if (n & 1) result = result * a % MOD;
      a = a * a % MOD;
      n >>= 1;
    }
    return result;
  }
  // copy from https://cp-algorithms.com/algebra/module-inverse.html
  static long inv(long a) {
    return a <= 1 ? a : MOD - (long)(MOD / a) * inv(MOD % a) % MOD;
  }

  int_mod operator+(const int_mod& b) const { return v + b.v; }
  int_mod& operator+=(const int_mod& b) {
    v = (v + b.v) % MOD;
    return *this;
  }
  int_mod operator-(const int_mod& b) const { return MOD + v - b.v; }
  int_mod operator*(const int_mod& b) const { return v * b.v; }
  int_mod operator/(const int_mod& b) const { return v * inv(b.v); }
  int_mod operator^(long b) const { return pow(v, b); }

  long v;
};
using intm = int_mod<1000000007>;

intm F[1000005];

int main() {
  std::ios_base::sync_with_stdio(0);
  int n, m, k;
  std::cin >> n >> k >> m;
  F[0] = 0;
  intm ik = intm(1) / k;
  F[2] = ik;
  for (int i = 1; i <= k; ++i) {
    F[i] = F[i - 1] * ik + F[i - 1] + ik;
  }
  for (int i = k + 1; i < m; ++i)
    F[i] = F[i - 1] * ik + F[i - 1] - F[i - k - 1] * ik;
  F[m] = 0;
  for (int i = 1; i <= k; ++i)
    if (m - i >= 0)
      F[m] = F[m] + (F[m - i] + 1) * (k - i + 1) * ik;
    else break;
      //F[m] = F[m] + ik * (k - i + 1);
  std::cout << (F[m] * n - n + 1).v << std::endl;
  return 0;
}