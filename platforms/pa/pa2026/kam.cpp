#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// #define DEBUG

#ifdef DEBUG
const int TT = 4;
#endif

const int MAX = 100005;

int P[MAX];

struct Node {
  int parent;
  std::unordered_set<int> children;
  int party;
  std::vector<int> E;
  bool processed;
  std::unordered_map<int, int> partiesToConnect;
  // std::queue<int> to_join;

  void clean() {
    processed = false;
    E.clear();
    children.clear();
    partiesToConnect.clear();
    // to_join = std::queue<int>();
  }
};

class Set {
 public:
  void clean(int n, int k) {
    for (int i = 1; i <= n; ++i) N[i].clean();
    for (int i = 1; i <= k; ++i) PartyClusters[i].clear();
    for (int i = 1; i <= k; ++i)
      enqueued[i] = true;  // avoid enqueuing before init
    remaining_parties.clear();
    Q = std::queue<int>();
  }
  int join(int u, int v) { return unionn(find(u), find(v)); }

  int unionn(int u, int v) {
    if (u == v) return v;
    if (N[u].partiesToConnect.size() > N[v].partiesToConnect.size()) return unionn(v, u);
    // if (N[u].party != N[v].party)
    //   std::clog << "Warning: different parties: " << u << "(" << N[u].party
    //             << ") vs " << v << "(" << N[v].party << ")" << std::endl;
    for (const auto& [party, w]: N[u].partiesToConnect) {
      int &vv = N[v].partiesToConnect[party];
      if (vv == 0) vv = w;
      else vv = join(vv, w);
    }
    N[u].partiesToConnect.clear();

    PartyClusters[N[u].party].erase(u);
    N[u].parent = v;
    N[v].children.insert(u);
    if (!enqueued[N[u].party] && PartyClusters[N[u].party].size() == 1 &&
        remaining_parties.count(N[u].party) > 0) {
      Q.push(v);
      enqueued[N[u].party] = true;
    }
    return v;
  }

  int find(int v) {
    int current_parrent = N[v].parent;
    if (current_parrent == v) return v;
    int new_parent = find(N[v].parent);

    N[current_parrent].children.erase(v);
    N[v].parent = new_parent;
    N[new_parent].children.insert(v);

    return new_parent;
  }

  int find_static(int v) {
    int current_parrent = N[v].parent;
    if (current_parrent == v) return v;
    return find_static(N[v].parent);
  }

  void reg(int v, int k) {
    N[v].party = k;
    N[v].parent = v;
    PartyClusters[k].insert(v);
    remaining_parties.insert(k);
  }

  void reg_edge(int u, int v) {
    N[u].E.push_back(v);
    N[v].E.push_back(u);
  }

  std::unordered_set<int> fetch_neighbours(int v,
                                           std::unordered_set<int> E = {}) {
    if (!N[v].processed)
      for (int w : N[v].children) E = fetch_neighbours(w, std::move(E));
    N[v].processed = true;
    // std::clog << "fetch_neighbours " << v << " :: ";
    // for (int w : N[v].E) std::clog << " " << find(w);
    // std::clog << std::endl;
    // for (int w : N[v].E)
    //   if (find_static(w) != v) E.insert(find_static(w));
    for (int w : N[v].E) E.insert(w);
    N[v].E.clear();
    // for (const auto& [party, w] : N[v].partiesToConnect)
    //   if (w != 0) E.insert(find(w));
    return std::move(E);
  }

  std::unordered_set<int> fetch_neighbours_filter(int v) {
    std::unordered_set<int> output;
    for (int w : fetch_neighbours(v))
      if (find(w) != v) output.insert(find(w));
    return output;
  }

  void init(int n, int k) {
    this->n = n;
    this->k = k;
    for (int i = 1; i <= k; ++i) enqueued[i] = false;
    for (int i = 1; i <= k; ++i) {
      if (!enqueued[i] && PartyClusters[i].size() == 1) {
        Q.push(*PartyClusters[i].begin());
        enqueued[i] = true;
      }
    }
  }

  void debug_print() {
    std::clog << "Clusters " << std::endl;
    for (int i = 1; i <= k; ++i) {
      std::clog << i << " :: ";
      for (int v : PartyClusters[i]) std::clog << " " << v;
      std::clog << std::endl;
    }
    std::clog << "Nodes " << std::endl;
    for (int i = 1; i <= n; ++i) {
      std::clog << i << " :: -> " << find(i) << std::endl;
    }
    std::clog << "Remaining:";
    for (int v : remaining_parties) std::clog << " " << v;
    std::clog << std::endl;

    std::clog << " ########### " << std::endl << std::endl;
  }

  Node N[MAX];
  std::unordered_set<int> PartyClusters[MAX];
  std::queue<int> Q;
  int n;
  int k;
  std::unordered_set<int> remaining_parties;
  bool enqueued[MAX];
} S;

bool solve(int t) {
  int n, m, k;
  std::cin >> n >> m >> k;

  S.clean(n, k);
  for (int i = 1; i <= n; ++i) {
    std::cin >> P[i];
    S.reg(i, P[i]);
#ifdef DEBUG
    if (t == TT) std::clog << " " << P[i];
#endif
  }
#ifdef DEBUG
  if (t == TT) std::clog << std::endl;
#endif
  for (int i = 0; i < m; ++i) {
    int u, v;
    std::cin >> u >> v;
    if (P[u] == P[v]) {
      S.join(u, v);
    } else {
      S.reg_edge(u, v);
    }
  }
  S.init(n, k);

#ifdef DEBUG
  if (t == TT) S.debug_print();
#endif

  while (!S.Q.empty()) {
    // process cluster to remove
    int kk = S.Q.front();
    int kp = P[kk];
    S.Q.pop();
#ifdef DEBUG
    if (t == TT)
      std::clog << "process cluster " << kk << " to remove" << std::endl;
#endif
    auto neighbours = S.fetch_neighbours_filter(kk);
    std::unordered_set<int> joined_neighbours;
    for (int u : neighbours) {
      if (S.remaining_parties.count(P[u]) == 0) {
        P[u] = kp;
        kk = S.join(u, kk);
        // for (int w : S.fetch_neighbours_filter(u))
        //   joined_neighbours.insert(S.find(w));
      }
    }
    std::unordered_map<int, int>& PartiesToConnect = S.N[kk].partiesToConnect;
    for (int u : neighbours) {
#ifdef DEBUG
      if (t == TT) std::clog << " :: fetchd neighbours: " << u << std::endl;
#endif

      int& v = PartiesToConnect[P[u]];
      if (v == 0)
        v = u;
      else
        v = S.join(v, u);
    }
#ifdef DEBUG
    if (t == TT)
      for (auto [party, w] : PartiesToConnect) {
        std::clog << "PartiesToConnect " << w << std::endl;
      }
#endif
    // S.N[kk].partiesToConnect = std::move(PartiesToConnect);
    // for (const auto& [party, w] : PartiesToConnect) {
    //   S.reg_edge(S.find(kk), S.find(w));
    // }

    S.remaining_parties.erase(kp);

#ifdef DEBUG
    if (t == TT) S.debug_print();
#endif
  }

  return S.remaining_parties.empty();
}

int main() {
  std::ios_base::sync_with_stdio(0);
  int T;
  std::cin >> T;
  for (int t = 1; t <= T; ++t)
    std::cout << (solve(t) ? "TAK" : "NIE") << std::endl;
  return 0;
}
