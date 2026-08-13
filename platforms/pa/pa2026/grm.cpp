#include <algorithm>
#include <iostream>
#include <vector>

const long MOD = 1000000007ll;
const long INF = MOD + 100;

// [a, b)
class Range {
 public:
  Range operator*(const Range& r) const {
    const Range& l = *this;
    return Range{.a = std::max(l.a, r.a), .b = std::min(l.b, r.b)};
  }

  bool operator&(const Range& r) const {
    const Range& l = *this;
    Range intersect = l * r;
    return intersect.a <= intersect.b;
  }

  bool operator&(const long v) const {
    if (v >= INF) return b == INF;
    return v >= a && v < b;
  }

  bool operator<(const Range& r) const {
    const Range& l = *this;
    return r.a <= l.b;
  }

  Range operator+(const Range& r) const {
    const Range& l = *this;
    return Range{.a = std::min(l.a, r.a), .b = std::max(l.b, r.b)};
  }

  bool operator==(const Range& r) const {
    const Range& l = *this;
    return r.a == l.a && r.b == l.b;
  }

  bool empty() { return a >= b && !(a == INF && b == INF); }
  long a, b;
};

// Range operator*(const LinFunc& f, const Range& r) const {
//   return Range{.a = f(r.a), .b = f(r.b)};
// }

class LinFunc {
 public:
  LinFunc() {}
  LinFunc(long aa, long bb) : a(aa), b(bb), a_m(aa), b_m(bb) {}
  // evaluation capped to infinity
  long val(long x) const { return std::min(a * x + b, INF); }
  long full_val(long x) const { return a * x + b; }
  Range full_val(Range r) const { return {full_val(r.a), full_val(r.b)}; }
  // inverted function
  long invval_floor(long x) const {
    if (x >= INF)
      return INF;
    else
      return (x - b) / a;
  }
  long invval_ceil(long x) const {
    if (x >= INF)
      return INF;
    else
      return (x - b + a - 1) / a;
  }
  // uncapped evalutaion, but modulo MOD
  long valmod(long x) const { return (a_m * x + b_m) % MOD; }
  // long lval() const {
  //     return val(l);
  // }
  // long rval() const {
  //     return val(r);
  // }

  long operator()(long x) const { return valmod(x); }
  Range operator()(const Range& r) const {
    return Range{.a = val(r.a), .b = val(r.b)};
  }

  Range operator/(const Range& r) const {
    return Range{.a = std::min(invval_ceil(r.a), INF),
                 .b = std::min(invval_ceil(r.b), INF)};
  }

  LinFunc operator*(const LinFunc& r) const {
    const LinFunc& l = *this;

    // x' = l.a*x+l.b
    // y = r.a*x'+r.b = r.a*(l.a*x+l.b)+r.b
    // y = (r.a*l.a)*x + (r.a*l.b+r.b)

    LinFunc out;
    out.a = std::min(r.a * l.a, INF);
    out.b = std::min(r.a * l.b + r.b, INF);
    out.a_m = r.a_m * l.a_m % MOD;
    out.b_m = (r.a_m * l.b_m + r.b_m) % MOD;
    return out;
  }

  long a, b;
  long a_m, b_m;
};

// Any value in range r will be modified by function f
class SubNode {
 public:
  LinFunc f;
  Range r;
};

class Node {
 public:
  Node sum(const Node& right_node) const {
    const Node& left_node = *this;

    Node result;
    // std::clog << "sum ... "<< std::endl;
    int li = 0;
    int ri = 0;
    bool end = false;
    for (const auto& [left_func, left_range] : left_node.F) {
      Range mid_range = left_func(left_range);
      // Range mid_range = left_func.full_val(left_range);
      // std::clog << "left_range " << left_range.a << ", " << left_range.b << " -> mid_range = " << mid_range.a << ", " << mid_range.b << std::endl;

      while (ri < right_node.F.size() && mid_range < right_node.F[ri].r) {
        Range cross = mid_range * right_node.F[ri].r;
        // std::clog << "cross (" << right_node.F[ri].r.a << ", " << right_node.F[ri].r.b << ")= " << cross.a << ", " << cross.b << std::endl;
        if (!cross.empty()) {
          Range cross_result;
          if (cross.a == INF) { cross_result = left_range; }
          else if (cross.b == INF) { cross_result = left_func / cross; cross_result.b = left_range.b; }
          else cross_result = left_func / cross;
          
          result.F.push_back(SubNode{.f = left_func * right_node.F[ri].f,
                                     .r = cross_result});
        }
        ++ri;
      }
      if (ri > 0) --ri;

      // for (const auto& [right_func, right_range] : right_node.F) {
      //   Range cross = mid_range * right_node.F[ri].r;
      //   if (!cross.empty()) {
      //     result.F.push_back(SubNode{.f = left_func * right_node.F[ri].f,
      //                                .r = left_func / cross});
      //   }
      // }
    }
    // if (result.F.empty()) {
    //   std::clog << "Empty sum?" << std::endl;
    //   std::clog << "Left: ";
    //   for (const auto& [left_func, left_range] : left_node.F) {
    //     std::clog << left_func.a << "x+" << left_func.b << " (" << left_range.a << ", " << left_range.b << ")" << std::endl;
    //   }
    //   std::clog << "Right: ";
    //   for (const auto& [left_func, left_range] : right_node.F) {
    //     std::clog << left_func.a << "x+" << left_func.b << " (" << left_range.a << ", " << left_range.b << ")" << std::endl;
    //   }

    // }
    return result;
  }
  void debug_print() const {
    for (const auto& [func, range] : F) {
      std::clog << func.a << "x+" << func.b << " in [" << range.a << ", "
                << range.b << ") / " << func.a_m << "x+" << func.b_m << std::endl;
    }
  }
  std::pair<long, long> eval(long x, long xcap) const {
    int l = 0;
    int r = F.size()-1;
    while (l<r) {
        int m = (l+r)/2;
        if (F[m].r & xcap) {
          l=r=m;
        } else if (F[m].r.b <= xcap) {
          l = m+1;
        } else {
          r = m;
        }
    }
    if (F.size() <= l) std::cerr << "ups: " << l << " " << F.size() << std::endl;
    // std::clog << "eval bin search: " << x << " (" << xcap << ") " << F[l].r.a << " " << F[l].r.b << " val " << F[l].f.a_m << std::endl;
    return {F[l].f.valmod(x), F[l].f.val(xcap)};
  }
  std::vector<SubNode> F;
};

// left adds, right multiplies
Node FromGate(long left, long right) {
  if (right == 1) {  // Always add
    SubNode node;
    node.f = LinFunc(1, left);
    node.r.a = 0;
    node.r.b = INF;

    return Node{.F = {node}};
  } else {  // Multiply if mid_point or more, otherwise add
    // ceil( left / (right - 1) )
    long mid_point = (left + right - 2) / (right - 1);
    SubNode left_node;
    left_node.f = LinFunc(1, left);
    left_node.r.a = 0;
    left_node.r.b = mid_point;

    SubNode right_node;
    right_node.f = LinFunc(right, 0);
    right_node.r.a = mid_point;
    right_node.r.b = INF;

    return Node{.F = {left_node, right_node}};
  }
}

const int NN = 1 << 19;
// const int N = 1 << 5;
int N = NN;

class Tree {
 public:
  void set(long x, long l, long r) { nodes[x + N] = FromGate(l, r); }
  void finalize() {
    for (int i = 0; i < N; ++i) ranges[i + N] = {i, i+1};
    for (int i = N - 1; i > 0; --i) {
      nodes[i] = nodes[2 * i].sum(nodes[2 * i + 1]);
      ranges[i] = ranges[2 * i] + ranges[2 * i + 1];
    }
  }
  std::pair<long, long> eval(long x, long xcap, long l, long r, int node) const {
    Range rr = {l, r};
    if (rr == ranges[node]) {
        // std::clog << "eval " << l << " - " << r << " (" << node << ") = " << x << " (" << xcap << ") -> " << nodes[node].eval(x, xcap).first << std::endl;
        return nodes[node].eval(x, xcap);
    } else {
        Range left = rr*ranges[node*2];
        Range right =  rr*ranges[node*2+1];
        if (!left.empty()) std::tie(x, xcap) = eval(x, xcap, left.a, left.b, node*2);
        if (!right.empty()) std::tie(x, xcap) = eval(x, xcap, right.a, right.b, node*2+1);
        return {x, xcap};
    }
  }
  Node nodes[2 * NN];
  Range ranges[2 * NN];
} tree;

int main() {
  std::ios_base::sync_with_stdio(0);

  long n, q;
  std::cin >> n >> q;
  while (N/2 > n) N /= 2;
  // std::clog << n << " vs " << N << std::endl;

  for (long i = 0; i < n; ++i) {
    long l, r;
    std::cin >> l >> r;
    tree.set(i, l, r);
  }

  for (long i = n; i < N; ++i) {
    tree.set(i, 0, 1);
  }
  tree.finalize();

  // DEBUG
  // long step = 1;
  // long step_count = 0;
  // for (long i = 1; i < 2 * N; ++i) {
  //   std::clog << "node " << i << std::endl;
  //   tree.nodes[i].debug_print();
  //   std::clog << std::endl;
  //   ++step_count;
  //   if (step_count >= step) {
  //     std::clog << std::endl << std::endl;
  //     step_count = 0;
  //     step *= 2;
  //   }
  // }

  for (long i = 0; i < q; ++i) {
    long x, l, r;
    std::cin >> x >> l >> r;
    // std::clog << "xlr (" << i+1 << ") = " << x << " " << l << " " << r << std::endl;
    std::cout << tree.eval(x, x, l, r, 1).first << std::endl;
    // std::clog << " ### " << std::endl;
  }

  return 0;
}
