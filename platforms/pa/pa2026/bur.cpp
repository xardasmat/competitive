#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#ifdef GEN
#include <cstdlib>
#include <ctime>
#include <fstream>
#endif

const int MAX = 100005;

long H[MAX];
long C[MAX];
bool isP[MAX];
std::vector<int> P;

void init(int n) {
  // Eratostenes sieve
  for (int i = 2; i <= n; ++i) isP[i] = true;
  for (int i = 2; i <= n; ++i) {
    if (isP[i]) {
      P.push_back(i);
      for (int j = 2 * i; j < MAX; j += i) isP[j] = false;
    }
  }
  std::reverse(P.begin(), P.end());
}

bool test(long k, int n) {
  // std::clog << "test " << k << std::endl;
  if (k > n) return false;
  long h = 0;
  for (int i = 0; i <= n; ++i) C[i] = 0;
  for (int i = 0; i < n; ++i) {
    h -= C[i];
    long diff = H[i] - h;
    if (diff < 0) return false;  // there are no negative waves
    if (i + k > n && diff != 0)
      return false;  // waves should not go out of boundary
    if (i + k <= n) C[i + k] = diff;
    h += diff;
  }
  h -= C[n];
  return h == 0;
}

std::vector<int> ps;
int nn;

int f(int i = 0, long k = 1) {
  if (k > nn) return 0;
  if (i >= ps.size()) return k;
  int best = 1;
  if (test(k * ps[i], nn)) {
    best = std::max(best, f(i + 1, k * ps[i]));
  }
  best = std::max(best, f(i + 1, k));
  return best;
}

int solve(int n) {
  ps.clear();
  for (int p : P)
    if (test(p, n)) {
      ps.push_back(p);
      long pp = p;
      while (test(pp * p, n)) {
        pp *= p;
        ps.push_back(pp);
      }
    }
  nn = n;
  // for (int ppp : ps) std::clog << "prime: " << ppp << std::endl;
  return f();
}

int solve2(int n, long sum) {
  ps.clear();
  long best = 1;
  for (long k = 1; k * k <= sum; ++k) {
    if (sum % k == 0) {
      if (test(sum / k, n)) best = std::max(best, sum / k);
      if (test(k, n)) best = std::max(best, k);
    }
  }
  return best;
}

#ifdef GEN

int brute(int n) {
  return 1;
  for (int i = n; i > 1; --i)
    if (test(i, n)) return i;
  return 1;
}

void gen(int t, int n, std::string dir) {
  std::clog << "tests/bur/" + dir + "/" + std::to_string(t) + ".in"
            << std::endl;
  std::ofstream in_file("tests/bur/" + dir + "/" + std::to_string(t) + ".in");
  std::ofstream out_file("tests/bur/" + dir + "/" + std::to_string(t) + ".out");
  int k = std::rand() % n + 1;
  std::clog << " :: " << k << std::endl;
  long h = 0;
  for (int i = 0; i < n; ++i) C[i] = 0;
  for (int i = 0; i < n; ++i) {
    // std::clog << h << std::endl;
    long diff = (i + k <= n) ? (std::rand() % 1000 + 1) : 0;
    for (int g = 0; g < 10 && h + diff > 1000000; ++g)
      diff = (i + k <= n) ? (std::rand() % 1000 + 1) : 0;
    if (h + diff > 100000) diff = 0;
    if (i + k <= n) C[i + k] = diff;
    h -= C[i];
    h += diff;
    H[i] = h;
  }

  in_file << n << std::endl;
  for (int i = 0; i < n; ++i) in_file << H[i] << std::endl;
  // out_file << brute(n) << std::endl;
  init(n);
  out_file << solve(n) << std::endl;
}
#endif

int main() {
  std::ios_base::sync_with_stdio(0);
#ifdef GEN
  std::srand(std::time(NULL));
  for (int i = 0; i < 10; ++i) gen(i, 100000, "big");
  return 0;
#endif

  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) std::cin >> H[i];
  long sum = 0;
  for (int i = 0; i < n; ++i) sum += H[i];
  // init(n);
  std::cout << solve2(n, sum) << std::endl;
  return 0;
}
