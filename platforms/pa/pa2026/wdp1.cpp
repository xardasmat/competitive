#include <algorithm>
#include <iostream>
#include <random>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #define DEBUG

const int MAXN = 10000000 + 5;  // 2*40MB
const int MAXQ = 1000000 + 5;   // 4*4MB
int A[MAXQ];                    // Query
int L[MAXQ];                    // Limit
int R[MAXQ];                    // Removal pointer
int IP[MAXQ];                   // Insertion pointer
int I[MAXN];                    // Insertion index
std::unordered_set<int> S;
std::vector<int> SV;
int SVI[MAXN];  // Vector Set index

std::unordered_set<int> DR[MAXQ];

std::random_device r;
std::default_random_engine e1(r());

long make_pair(int x, int y) { return long(x) | (long(y) << 32); }

std::unordered_map<long, int> M;

std::multiset<int, std::greater<int>> V;

std::vector<int> P;             // 40MB
const int MAXP = 10000000 + 5;  // 10000000 * 2 / 3; // 10MB + 40MB
bool nonP[MAXP + 5];
int dividor[MAXP + 5];

void init(int n) {
  // Eratostenes sieve
  for (int i = 2; i <= n; ++i) {
    if (!nonP[i]) {
      P.push_back(i);
      dividor[i] = i;
      for (long j = 2 * i; j <= n; j += i) {
        nonP[j] = true;
        dividor[j] = i;
      }
    }
  }
}

const int MAXT = 1 << 20;

class Range {
 public:
  void set(int i, int v) { V[i + MAXT] = v; }

  void init() {
    for (int i = MAXT - 1; i > 0; --i) V[i] = std::min(V[2 * i], V[2 * i + 1]);
  }

  int query(int a, int b) {
    a += MAXT;
    b += MAXT;
    int result = std::min(V[a], V[b]);
    while (a / 2 < b / 2) {
      if (!(a & 1)) result = std::min(result, V[a + 1]);
      if (b & 1) result = std::min(result, V[b - 1]);
      a /= 2;
      b /= 2;
    }
    return result;
  }

  int V[2 * MAXT];
} rt;  // 8MB

const int MAXD = 1 << 20;

class RangeDiv {
 public:
  void set(int i, const std::unordered_set<int>& D) {
    for (int p : D) V[i + MAXD].push_back(p);
    std::sort(V[i + MAXD].begin(), V[i + MAXD].end());
  }

  void init() {
    for (int i = MAXD - 1; i > 0; --i) {
      std::merge(V[2 * i].begin(), V[2 * i].end(), V[2 * i + 1].begin(),
                 V[2 * i + 1].end(), std::back_inserter(V[i]));
      V[i].erase(std::unique(V[i].begin(), V[i].end()), V[i].end());
    }
  }

  std::unordered_set<int> query(int a, int b) {
    a += MAXD;
    b += MAXD;
    std::unordered_set<int> result;
    for (int p : V[a]) result.insert(p);
    for (int p : V[b]) result.insert(p);
    while (a / 2 < b / 2) {
      if (!(a & 1))
        for (int p : V[a + 1]) result.insert(p);
      if (b & 1)
        for (int p : V[b - 1]) result.insert(p);
      a /= 2;
      b /= 2;
    }
    return result;
  }

  std::vector<int> V[2 * MAXD];
} rtd;

void add_div(int x, std::unordered_set<int>& D, int limit) {
  while (x > 1) {
    if (dividor[x] <= limit) D.insert(dividor[x]);
    x /= dividor[x];
  }
}

const double error_treshold = 1e-8;

std::unordered_set<int> stohastic_dividors(int limit) {
  std::unordered_set<int> D;
  if (limit < 1) {
    for (int i = 0; i < P.size(); ++i) {
      if (P[i] > limit) break;
      D.insert(P[i]);
    }
    return D;
  }
  if (SV.size() < 10) {  // get all dividors
    for (int i = 0; i < SV.size(); ++i)
      for (int j = i + 1; j < SV.size(); ++j) {
        add_div(std::abs(SV[i] - SV[j]), D, limit);
      }
    return D;
  }
  std::uniform_int_distribution<int> uniform_dist(0, SV.size() - 1);
  double error_prob = 1;
  while (error_prob > error_treshold) {
    int l = uniform_dist(e1);
    int r = uniform_dist(e1);
    if (l == r) continue;
    add_div(std::abs(l - r), D, limit);
    error_prob *= 0.75;
  }
  return D;
}

void add(int a, int i) {
  // auto& PP = DR[R[i]] = stohastic_dividors(L[i]);
  for (int p : DR[i]) {
    // if (p > L[i]) break;
    int& v = M[make_pair(p, a % p)];
    if (v > 0) V.erase(V.find(v));
    ++v;
    V.insert(v);
  }
}

void remove(int a, int i) {
  for (int p : DR[R[i]]) {
    // if (p > L[IP[i]]) break;
    int& v = M[make_pair(p, a % p)];
    V.erase(V.find(v));
    --v;
    if (v > 0) V.insert(v);
  }
}

void pre_add(int a) {}
void pre_remove(int a) {}

int SB[20];

int limit(int n, int d) { return (2 * n + d - 1) / d; }

int test_brute(int n) {
  if (S.size() <= 1) return S.size();
  int k = 0;
  for (int a : S) SB[k++] = a;
  if (abs(SB[0] - SB[1]) == 1)
    return 1;
  else
    return 2;
  // std::unordered_set<int> D;
  // for (int i = 0; i < k; ++i) {
  //   for (int j = i + 1; j < k; ++j) {
  //     add_div(std::abs(SB[i] - SB[j]), D, limit(n, S.size()));
  //   }
  // }
  // int best = 1;
  // std::unordered_map<long, int> M;
  // for (int i = 0; i < k; ++i)
  //   for (int p : D) {
  //     int v = ++M[make_pair(p, SB[i] % p)];
  //     best = std::max(best, v);
  //   }
  // return best;
}

int solve(int i, int n) {
  if (S.size() <= 2) return test_brute(n);
  if (V.empty())
    return 0;
  else
    return *V.begin();
}

int main() {
  std::ios_base::sync_with_stdio(0);

  int q;
  int n;
  std::cin >> n >> q;
  //   init(std::max(n / 10, 1000l));
  // init((n * 2 + 2) / 3);
  init(n);
  //   std::clog << "P.size() = " << P.size() << std::endl;
  for (int i = 0; i < q; ++i) {
    std::cin >> A[i];
    R[i] = q;
    if (S.count(A[i])) {
      R[I[A[i]]] = i;   // R[i] points to removal of the stone
      IP[i] = I[A[i]];  // IP[i] points to insertion of the stone
      pre_remove(A[i]);
      S.erase(A[i]);
    } else {
      S.insert(A[i]);
      pre_add(A[i]);
      I[A[i]] = i;
    }
    rt.set(i, S.size());
  }
  rt.init();
  S.clear();
  for (int i = 0; i < q; ++i) {
    L[i] = limit(n, rt.query(i, R[i] - 1));
    if (S.count(A[i])) {
      S.erase(A[i]);
      int SVback = SV.back();
      std::swap(SV[SVI[A[i]]], SV.back());
      std::swap(SVI[A[i]], SVI[SVback]);
      SV.pop_back();
    } else {
      S.insert(A[i]);
      SVI[A[i]] = SV.size();
      SV.push_back(A[i]);
      add(A[i], i);
    }
    rtd.set(i, stohastic_dividors(L[i]));
  }
  rtd.init();

  S.clear();
  for (int i = 0; i < q; ++i) {
    DR[i] = rtd.query(i, R[i] - 1);
    if (S.count(A[i])) {
      remove(A[i], i);
      S.erase(A[i]);
    } else {
      S.insert(A[i]);
      add(A[i], i);
    }
    std::cout << solve(i, n) << "\n";
  }
#ifdef DEBUG
  for (int i = 0; i < q; ++i) {
    std::clog << " " << R[i];
  }
  std::clog << std::endl;
  for (int i = 0; i < q; ++i) {
    std::clog << " " << L[i];
  }
  std::clog << std::endl;
#endif
  return 0;
}
