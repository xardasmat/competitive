#define DEBUG
#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>
#ifdef DEBUG
#include <iomanip>
#endif

// const long MOD = 1000000007;
const int MAX = 1000005;

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

intm F[MAX * 4];
intm iF[MAX * 4];

void init() {
  F[0] = 1;
  for (long i = 1; i < MAX * 4; ++i) F[i] = F[i - 1] * i;
  // for (long i = 1; i < MAX * 4; ++i) iF[i] = inv(F[i]);
}

intm C(long k, long n) { return F[n] / (F[k] * F[n - k]); }

// Win all matches
// team A has both top and second top card
intm zeros(long n) {
  return C(2 * n - 2, 4 * n - 2) * F[2 * n] * F[2 * n] / (intm(4) ^ n);
}

// Tie all matches
// team A has top card, team B has second and third card
intm ones(long n) {
  return C(2 * n - 1, 4 * n - 3) * F[2 * n] * F[2 * n] / (intm(4) ^ n) * 2;
}

// There are k segments of ones
// team A has top k even cards, team B has top k odd cards in order.
//
// E.g. K=2
//
// 0001110001111100
//   D  C  B    A
//  D -> C -> B -> A
//    -> E
//  E(+F) -> D -> C -> B -> A
// OR
// 0001110001111100
//   D  C  B  A E
//  E -> D -> C -> B -> A
//    -> F
//  F(+G) -> E -> D -> C -> B -> A
// OR
// 0001110001111100
//   D  C  BA   E
intm segment(long segments, long ones, long zeros, long n) {
  intm total = 0;
  intm div = intm(1) / intm(4) ^ n;
  intm base = intm(2) ^ (segments * 2);
  long free = 4 * n - segments * 2;
  // first case, i.e. no fixed E
  total += base * ((2 * n - zeros + 1) / 2 * 2 - segments) * F[free - 1] * div;
  total += base * (zeros / 2 * 2) * (2 * n - segments - 1) * F[free - 2] * div;

  // second case, i.e. fixed E
  intm base2 = base * 2;
  long new_ones = 0;
  while (ones > 1) {
    ones -= 2;
    new_ones += 2;
    total += base2 * ((2 * n - new_ones) - segments) * F[free - 2] * div;
    total += base2 * new_ones * (2 * n - segments - 1) * F[free - 3] * div;
  }

  return total;
}

intm segments(const std::vector<int>& C, int start, int n) {
  intm total = 0;
  for (int i = start; i < C.size(); i += 2) {
    total +=
        segment(C.size() / 2, C[i], (i + 1) >= C.size() ? C[0] : C[i + 1], n);
  }
  return total;
}

int A[MAX * 2];

struct Score {
  std::vector<int> C;
  bool broken;
};

Score CountScore(int n) {
  Score score = {.C = {1}, .broken = false};
  int non_one = A[0];

  for (int i = 1; i < 2 * n; ++i) {
    if (A[i] != 1) {
      if (non_one == 1)
        non_one = A[i];
      else if (non_one != A[i])
        score.broken = true;
    }
    if (A[i - 1] != A[i]) {
      if (A[i] == 0 && i % 2 != 1) score.broken |= true;
      if (A[i] == 2 && i % 2 != 0) score.broken |= true;
      score.C.push_back(1);
    } else
      score.C.back()++;
  }
  if (score.C.size() > 1 && A[0] == A[2 * n - 1]) {
    score.C.front() += score.C.back();
    score.C.pop_back();
  } else if (score.C.size() > 1) {
    if (A[0] == 0 && 0 % 2 != 1) score.broken |= true;
  }
  if (score.C.size() > 1)
    for (int v : score.C) score.broken |= !(v & 1);
  return score;
}

void solve() {
  long n;
  std::cin >> n;
  for (int i = 0; i < 2 * n; ++i) std::cin >> A[i];
  Score score = CountScore(n);

  if (score.broken) {
    std::cout << 0 << "\n";
  } else if (score.C.size() == 1) {
    if (A[0] == 0 || A[0] == 2)
      std::cout << zeros(n).v << "\n";
    else
      std::cout << ones(n).v << "\n";
  } else {
    std::cout << segments(score.C, A[0] == 1 ? 0 : 1, n).v << "\n";
  }
}

int main() {
  std::ios_base::sync_with_stdio(0);
  init();
  int t;
  std::cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
