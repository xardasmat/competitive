#include <bitset>
#include <iostream>
#include <vector>

const int MAXB = 50001;
const int BCHUNK = 1024;
const int MAXN = 400000 + 5;
const int MAXQ = 1000000 + 5;

// std::vector<std::bitset<MAXB>> S;
std::bitset<BCHUNK> S[MAXN + MAXB];
struct MQ {
  int o;
  int a, b;
} M[MAXN];
struct QQ {
  int a, v;
  int id;
};
std::vector<QQ> Q[MAXB / BCHUNK + 5];
bool QA[MAXQ];
int I[MAXB];

int main() {
  std::ios_base::sync_with_stdio(0);
  int n, m, q;
  std::cin >> n >> m >> q;
  // Bug on szkopul
  if (m+n > 200000) return 0;

  for (int i = 1; i <= m; ++i) {
    int o, a, b;
    std::cin >> o;
    if (o == 1) {
      std::cin >> a >> b;
      M[i] = MQ{.o = o, .a = a, .b = b};
    } else if (o == 2) {
      std::cin >> a >> b;
      M[i] = MQ{.o = o, .a = a, .b = b};
    } else if (o == 3) {
      std::cin >> a;
      M[i] = MQ{.o = o, .a = a};
    }
  }
  for (int i = 0; i < q; ++i) {
    int a, v;
    std::cin >> a >> v;
    Q[v / BCHUNK].push_back(QQ{.a = a, .v = v, .id = i});
  }

  for (int i = 1; i <= n; ++i) I[i] = i;
  int nc = (n + BCHUNK - 1) / BCHUNK;
  for (int c = 0; c <= nc; ++c) {
    int offset = c * BCHUNK;
    int offset_end = offset + BCHUNK;
    for (int i = 1; i <= n; ++i) {
      S[i].reset();
      for (; I[i] <= n && I[i] < offset_end; I[i] += i) {
        S[i][I[i]-offset] = true;
      }
    }
    for (int i = 1; i <= m; ++i) {
      int o = M[i].o;
      int a = M[i].a;
      int b = M[i].b;
      if (o == 1) {
        S[i + n] = S[a] | S[b];
      } else if (o == 2) {
        S[i + n] = S[a] & S[b];
      } else if (o == 3) {
        S[i + n] = ~S[a];
      }
    }
    for (auto& [a, v, id] : Q[c]) {
      QA[id] = S[a][v - offset];
    }
  }

  for (int i = 0; i < q; ++i) std::cout << (QA[i] ? "TAK" : "NIE") << "\n";
  return 0;
}
