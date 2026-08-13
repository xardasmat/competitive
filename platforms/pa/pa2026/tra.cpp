#include <iostream>
#include <set>
#include <vector>

const int MAX = 2001;
const int MAXC = 500;
const int INF = 2*MAX * MAXC + 5;

std::vector<std::pair<int, int>> E[MAX];

int C[MAX];
struct cmp {
  bool operator()(int a, int b) const {
    return std::tie(C[a], a) < std::tie(C[b], b);
  }
};

void Send(int v, int bits) {
  for (int i = 0; i < bits; ++i) {
    std::cout << "+ " << (v & 1 ? 1 : 0);
    v /= 2;
  }
  std::cout << std::flush;
}

int Receive(int bits) {
  int v = 0;
  for (int i = 0; i < bits; ++i) {
    std::cout << "?" << std::flush;
    char bit;
    std::cin >> bit;
    v |= (bit == '1' ? 1 : 0) << i;
  }
  return v;
}

int Exchange(bool master, int front) {
    if (front > 511) front = 511;
    if (master) {
        Send(front, 9);
        return Receive(9);
    } else {
        int v = Receive(9);
        Send(front, 9);
        return v;
    }
}

int main() {
  std::ios_base::sync_with_stdio(0);
  std::string role;
  std::cin >> role;
  bool master = role == "Algosia";
  int n, m;
  std::cin >> n >> m;
  for (int i = 0; i < m; ++i) {
    int a, b, c;
    std::cin >> a >> b >> c;
    E[a].push_back({b, c});
    E[b].push_back({a, c});
  }
  for (int i = 1; i <= n; ++i) C[i] = INF;
  C[1] = 0;
  std::set<int, cmp> Q;
  for (int i = 1; i <= n; ++i) Q.insert(i);
  int front = 0;
  while (!Q.empty()) {
    int u = *Q.begin();
    int other_front = Exchange(master, C[u] - front)+front;
    if (other_front < C[u] || other_front == C[u] && !master) {
        u = Receive(11);
        front = other_front;
    } else {
        Send(u, 11);
        front = C[u];
    }

    Q.erase(u);
    C[u] = front;
    for (const auto& [v, c] : E[u]) {
      if (C[v] > C[u] + c) {
        Q.erase(v);
        C[v] = C[u] + c;
        Q.insert(v);
      }
    }
  }

  if (master) {
    std::cout << "!";
    for (int i=1;i<=n;++i) std::cout << " " << C[i];
    std::cout << std::flush;
  }

  return 0;
}
