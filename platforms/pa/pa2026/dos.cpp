#include <iostream>
#include <set>
#include <vector>

const int MAX = 1005;

int H[MAX];

struct Node {
  int index;
  bool operator<(const Node& node) const {
    return std::tie(H[index], index) > std::tie(H[node.index], node.index);
  }
};

std::set<Node> S;

int k = 0;
long update(int a, int b) {
  long diff = H[a] - H[b];
  if (diff > k) {
    S.erase(S.find(Node{b}));
    H[b] += diff - k;
    S.insert(Node{b});
    return diff - k;
  } else {
    return 0;
  }
}

int main() {
  std::ios_base::sync_with_stdio(0);
  int n;
  std::cin >> n >> k;
  for (int i = 1; i <= n; ++i) {
    std::cin >> H[i];
    S.insert(Node{i});
  }
  long total = 0;
  while (!S.empty()) {
    auto it = S.begin();
    int i = it->index;
    S.erase(it);
    if (i+1 <= n)
        total += update(i, i+1);
    if (i-1 > 0)
        total += update(i, i-1);
  }
  std::cout << total << std::endl;
  return 0;
}
