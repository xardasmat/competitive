#include <algorithm>
#include <iostream>
#include <set>
#include <vector>

// #define DEBUG

const int MAX = 300000 * 2;
const long INF = MAX * 1000000000000ll;

int n, m, k;

int index(int i, int j) { return i * (m + 1) + j; }

int dn;

long P[MAX];
bool G[MAX];
long S[MAX];

long SP[MAX];
long SA[MAX];
long SAM[MAX];
long SB[MAX];
long SBM[MAX];

int LI[MAX];

std::vector<long> E;

bool cmp(int a, int b) {
  return std::make_tuple(G[a], -S[a]) < std::make_tuple(G[b], -S[b]);
}

long solve(int dn) {
  int full = dn / m;
  int rem = dn % m;

  long result = std::max(SP[full + 1] - SAM[index(full + 1, rem)],
                         SP[full] + SBM[index(full, rem)]);
#ifdef DEBUG
  std::clog << "solve " << dn << " :: " << "full=" << full << "; rem=" << rem
            << "; result=" << result << std::endl;
#endif
  return result;
}

void init() {
  for (int i = 0; i < dn; ++i) SP[i + 1] = S[LI[i]] + SP[i];

  for (int j = 1; j <= m; ++j) {
    for (int i = dn - 1; i >= 0; --i) {
      SB[index(i, j)] = SB[index(i, j - 1)] + P[index(LI[i], j - 1)];
      SBM[index(i, j)] = std::max(SBM[index(i + 1, j)], SB[index(i, j)]);
    }
  }

  for (int j = m; j >= 0; --j) SAM[index(0, j)] = INF;
  for (int j = m - 1; j >= 0; --j) {
    for (int i = 1; i <= dn; ++i) {
      SA[index(i, j)] = SA[index(i, j + 1)] + P[index(LI[i-1], j)];
      SAM[index(i, j)] = std::min(SAM[index(i - 1, j)], SA[index(i, j)]);
    }
  }
#ifdef DEBUG
  std::clog << "SP :: " << std::endl;
  for (int i = 0; i <= n; ++i) std::clog << " " << SP[i];
  std::clog << std::endl << std::endl;

  std::clog << "SB :: " << std::endl;
  for (int j = 0; j <= m; ++j) {
    for (int i = 0; i <= dn; ++i) std::clog << " " << SB[index(i, j)] << " ";
    std::clog << std::endl;
  }
  std::clog << std::endl;

  std::clog << "SBM :: " << std::endl;
  for (int j = 0; j <= m; ++j) {
    for (int i = 0; i <= dn; ++i) std::clog << " " << SBM[index(i, j)] << " ";
    std::clog << std::endl;
  }
  std::clog << std::endl;

  std::clog << "SA :: " << std::endl;
  for (int j = 0; j <= m; ++j) {
    for (int i = 0; i <= dn; ++i) std::clog << " " << SA[index(i, j)] << " ";
    std::clog << std::endl;
  }
  std::clog << std::endl;

  std::clog << "SAM :: " << std::endl;
  for (int j = 0; j <= m; ++j) {
    for (int i = 0; i <= dn; ++i) std::clog << " " << SAM[index(i, j)] << " ";
    std::clog << std::endl;
  }
  std::clog << std::endl;
#endif
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::cin >> n >> m >> k;
  for (int i = 0; i < n; ++i) {
    G[i] = true;
    LI[i] = i;
    for (int j = 0; j < m; ++j) {
      std::cin >> P[index(i, j)];
      if (j > 0) G[i] &= P[index(i, j)] <= P[index(i, j - 1)];
      S[i] += P[index(i, j)];
    }
  }

  if (m == 1) {
    std::sort(S, S + n);
    long total = 0;
    for (int i = 1; i <= k; ++i) total += S[n - i];
    std::cout << total << std::endl;
    return 0;
  }

  for (int i = 0; i < n; ++i) {
    if (G[i]) {
      for (int j = 0; j < m; ++j) {
        E.push_back(P[index(i, j)]);
      }
    } else {
    }
  }
  std::sort(LI, LI + n, cmp);
  std::sort(E.begin(), E.end(), std::greater<long>());

  int gn = E.size();
  dn = n - gn / m;
  int dnm = n * m - gn;
  int dnmk = std::min(dnm, k);
  int ln = std::min(k, gn);
  int rn = k - ln;

  long total = 0;
  for (int i = 0; i < ln; ++i) total += E[i];

  init();
  long best = solve(rn) + total;

  while (ln > 0 && rn < dnmk) {
    --ln;
    ++rn;
    total -= E[ln];
    best = std::max(solve(rn) + total, best);
  }
  std::cout << best << std::endl;
  return 0;
}
