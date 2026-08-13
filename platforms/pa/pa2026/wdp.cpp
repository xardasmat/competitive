#pragma GCC optimize("O3")

#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #define DEBUG

const int DENS = 1000;
const int MAXN = 10000000 + 5;
const int MAXQ = 1000000 + 5;
int A[MAXQ];  // Query
// int L[MAXQ];   // Limit
// int R[MAXQ];   // Removal pointer
// int IP[MAXQ];  // Insertion pointer
// int I[MAXN];   // Insertion index

int NN;

std::set<int> S;            // Stones
std::unordered_set<int> P;  // Primes - candidates for a step

int limit(int n, int d) {
  if (d == 0) return 0;
  return (2 * n + d - 1) / d;
}

long make_pair(int x, int y) { return long(x) | (long(y) << 32); }

template <int N>
class LayerTracker {
 public:
  int& Find(int p, int a) {
    if (p >= N) return M[make_pair(p, a % p)];
    return VM[p][a % p];
  }
  void Remove(int p, int a) {
    if (p >= N) M.erase(make_pair(p, a % p));
  }

  std::unordered_map<long, int> M;  // Sparse map of layers
  int VM[N][N];                     // Dense map of layers
};

LayerTracker<DENS> layers;

template <int N>
class ScoreTracker {
 public:
  void Increment(int& v) {
    if (v > 0) V[v]--;
    ++v;
    V[v]++;
    if (v > bestV) bestV = v;
  }
  void Decrement(int& v) {
    V[v]--;
    if (bestV == v && V[v] == 0) --bestV;
    --v;
    if (v > 0) V[v]++;
  }
  int Best() const { return bestV; }

  int V[N];
  int bestV = 0;
};

ScoreTracker<MAXQ> score;

template <int N>
class UberScoreTracker {
 public:
  UberScoreTracker() {
    for (int i = 0; i < N; ++i) I[i] = Vs.end();
  }

  void Increment(int& v) {
    if (v > 0) V[v]--;
    ++v;
    V[v]++;
    EnsureNext(v - 1);
    CleanList(v - 1);
  }

  void Decrement(int& v) {
    V[v]--;
    --v;
    if (v > 0) {
      V[v]++;
      EnsurePrev(v + 1);
    }
    CleanList(v + 1);
  }
  const std::list<int>& Best() const { return Vs; }

  void EnsureNext(int v) {
    if (I[v + 1] != Vs.end()) return;
    if (v == 0) {
      Vs.push_front(1);
      I[1] = Vs.begin();
    } else {
      auto it = I[v];
      ++it;
      I[v + 1] = Vs.insert(it, v + 1);
    }
  }
  void EnsurePrev(int v) {
    if (I[v - 1] != Vs.end()) return;
    I[v - 1] = Vs.insert(I[v], v - 1);
  }

  void CleanList(int v) {
    if (v == 0) return;
    if (V[v] != 0) return;
    Vs.erase(I[v]);
    I[v] = Vs.end();
  }

  int V[N];
  std::list<int>::iterator I[N];
  std::list<int> Vs;
};

template <typename K, int N>
class CandidateTracker {
 public:
  int& Find(K k) {
    int p = k & 0xffffffff;
    if (p >= DENS) return SM[k];
    return VM[p][k >> 32];
  }
  void Clean(K k) {
    if ((k & 0xffffffff) >= DENS) SM.erase(k);
  }
  void Increment(K k) {
    int& v = Find(k);
    if (v > 0) SK[v].erase(k);
    T.Increment(v);
    SK[v].insert(k);
  }
  void Decrement(K k) {
    int& v = Find(k);
    SK[v].erase(k);
    T.Decrement(v);
    if (v > 0)
      SK[v].insert(k);
    else
      Clean(k);
  }
  std::unordered_set<K> Best(int threshold) {
    std::unordered_set<K> V;
    auto& L = T.Best();
    for (auto it = L.rbegin(); it != L.rend(); ++it) {
      if (*it < threshold) break;
      for (int v : SK[*it]) V.insert(v & 0xffffffff);
    }
    return V;
  }

  std::unordered_map<K, int> SM;
  int VM[DENS][DENS];
  std::unordered_set<K> SK[N];
  UberScoreTracker<N> T;
};
CandidateTracker<long, MAXQ * 2> candidates;

const int MAXP = 10000000;  // 10000000 * 2 / 3;
bool nonP[MAXP + 5];
int dividor[MAXP + 5];
// int nextDiv[MAXP + 5];

void init(int n) {
  // Eratostenes sieve
  for (int i = 3; i <= n; i += 2) {
    if (!nonP[i]) {
      dividor[i] = i;
      // nextDiv[i] = 1;
      int i2 = i + i;
      // for (long k = 3, j = 3 * i; j <= n; k += 2, j += i2) {
      for (long j = 3 * i; j <= n; j += i2) {
        nonP[j] = true;
        dividor[j] = i;
        // nextDiv[j] = k;
        // if (nextDiv[j] % i == 0) nextDiv[j] = nextDiv[nextDiv[j]];
      }
    }
  }
}

std::vector<int> Divs[MAXN + 5];

const std::vector<int>& dividers(int n) {
  std::vector<int>& d = Divs[n];
  if (!d.empty()) return d;
  d.reserve(8);
  // if (!(n & 1)) d.push_back(2);
  // n >>= __builtin_ctz(n);
  while (n > 1) {
    int dd = dividor[n];
    d.push_back(dd);
    do {
      // n = nextDiv[n];
      n /= dd;
    } while (n % dd == 0);
  }
  return d;
}

void AddStonePrime(int a, int p) {
  int& v = layers.Find(p, a);
  score.Increment(v);
}

void RemoveStonePrime(int a, int p) {
  int& v = layers.Find(p, a);
  score.Decrement(v);
  if (v == 0) layers.Remove(p, a);
}

void AddPrime(int p) {
  auto [it, inserted] = P.insert(p);
  if (!inserted) return;
  for (int a : S) AddStonePrime(a, p);
}

void RemovePrime(int p) {
  if (P.count(p) == 0) return;
  P.erase(p);
  for (int a : S) RemoveStonePrime(a, p);
}

std::tuple<std::set<int>::iterator, std::set<int>::iterator,
           std::set<int>::iterator, std::set<int>::iterator,
           std::set<int>::iterator>
GetIters(std::set<int>::iterator it) {
  auto b = it;
  auto c = it;
  ++c;
  auto d = c;
  if (it == S.begin())
    b = S.end();
  else
    --b;
  if (d != S.end()) ++d;
  auto a = b;
  if (a == S.begin() || a == S.end())
    a = S.end();
  else
    --a;
  return {a, b, it, c, d};
}

void DelPair(std::set<int>::iterator l, std::set<int>::iterator r) {
  if (l != S.end() && r != S.end()) {
    int dif = std::abs(*l - *r);
    // if (!(dif & 1)) candidates.Decrement(make_pair(2, *l % 2));
    dif >>= __builtin_ctz(dif);
    // while (dif > 1) {
    //   int p = dividor[dif];
    //   // do { dif = nextDiv[dif]; } while (dif % p == 0);
    //   do {
    //     dif /= p;
    //   } while (dif % p == 0);
    //   // dif = nextDiv[dif];
    //   candidates.Decrement(make_pair(p, *l % p));
    // }

    for (int p : dividers(dif)) candidates.Decrement(make_pair(p, *l % p));
  }
}
void AddPair(std::set<int>::iterator l, std::set<int>::iterator r) {
  if (l != S.end() && r != S.end()) {
    int dif = std::abs(*l - *r);
    // if (!(dif & 1)) candidates.Increment(make_pair(2, *l % 2));
    dif >>= __builtin_ctz(dif);
    // while (dif > 1) {
    //   int p = dividor[dif];
    //   // do { dif = nextDiv[dif]; } while (dif % p == 0);
    //   do {
    //     dif /= p;
    //   } while (dif % p == 0);
    //   // dif = nextDiv[dif];
    //   candidates.Increment(make_pair(p, *l % p));
    // }
    for (int p : dividers(dif)) candidates.Increment(make_pair(p, *l % p));
  }
}
// a b c d
// ab + ac + bc + bd + cd ..
// del: ac, bd
// a b X c d
// ab + aX + bX + bc + Xc + Xd + cd ..
// add: aX, bX, Xc, Xd
void AddStone(std::set<int>::iterator it) {
  auto [a, b, X, c, d] = GetIters(it);
  DelPair(a, c);
  DelPair(b, d);
  AddPair(a, X);
  AddPair(b, X);
  AddPair(X, c);
  AddPair(X, d);
  for (int p : P) AddStonePrime(*it, p);

  for (int p : candidates.Best(S.size() / 4)) AddPrime(p);
  auto P2 = P;
  auto nP = candidates.Best(S.size() / 8);
  for (int p : P2)
    if (p != 2 && nP.count(p) == 0) RemovePrime(p);
}

void RemoveStone(std::set<int>::iterator it) {
  auto [a, b, X, c, d] = GetIters(it);

  DelPair(a, X);
  DelPair(b, X);
  DelPair(X, c);
  DelPair(X, d);
  AddPair(a, c);
  AddPair(b, d);

  for (int p : candidates.Best(S.size() / 4)) AddPrime(p);
  auto P2 = P;
  auto nP = candidates.Best(S.size() / 8);
  for (int p : P2)
    if (p != 2 && nP.count(p) == 0) RemovePrime(p);

  for (int p : P) RemoveStonePrime(*it, p);
}

int SB[20];

int test_brute(int n) {
  if (S.size() <= 1) return S.size();
  int k = 0;
  for (int a : S) SB[k++] = a;
  if (abs(SB[0] - SB[1]) == 1)
    return 1;
  else
    return 2;
}

int solve(int n) {
  if (S.size() <= 2) return test_brute(n);
  return score.Best();
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin.tie(nullptr);

  int q;
  int n;
  std::cin >> n >> q;
#ifdef DEBUG
  q = 100;
#endif
  for (int i = 0; i < q; ++i) std::cin >> A[i];
  NN = n;
  init(n);
  AddPrime(2);
  for (int i = 0; i < q; ++i) {
#ifdef DEBUG
    std::clog << A[i] << std::endl;
#endif
    auto [it, ins] = S.insert(A[i]);
    if (!ins) {
      RemoveStone(it);
      S.erase(it);
    } else {
      AddStone(it);
    }
#ifdef DEBUG
    std::clog << " ### " << i << std::endl;
    std::clog << " layers:";
    for (const auto& [k, v] : layers.M) {
      std::clog << " " << (k >> 32) << "/" << (k & 0xffffffff) << "=" << v;
    }
    // std::clog << " candidates:";
    // for (const auto& k : candidates.SM) {
    //   std::clog << " " << (k >> 32) << "/" << (k & 0xffffffff) << "="
    //             << candidates.SM[k];
    // }
    std::clog << std::endl;
    std::clog << " candidatesBest:";
    for (const auto& p : candidates.Best(S.size() / 4)) {
      std::clog << " " << p;
    }
    std::clog << std::endl;
    std::clog << " candidatesBest2:";
    for (const auto& p : candidates.Best(S.size() / 4)) {
      std::clog << " " << p;
    }
    std::clog << std::endl;

    std::clog << "A:";
    for (const auto& p : S) {
      std::clog << " " << p;
    }
    std::clog << std::endl;

    std::clog << " primes:";
    for (const auto& p : P) {
      std::clog << " " << p;
    }
    std::clog << std::endl;
#endif
    A[i] = solve(n);
  }
  for (int i = 0; i < q; ++i) std::cout << A[i] << "\n";
  return 0;
}
