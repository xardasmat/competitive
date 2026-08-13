#include <iostream>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>

template <int K>
class Matrix {
 public:
  void swap_row(int i, int j) {
    if (i > j) {
      swap_row(j, i);
      return;
    }
    auto ones_stripe = (__uint128_t(1) << K) - 1;
    auto row1_stripe = ones_stripe << (i * K);
    auto row2_stripe = ones_stripe << (j * K);
    row1_stripe &= bits;
    row2_stripe &= bits;
    bits ^= row1_stripe;
    bits ^= row2_stripe;
    row1_stripe <<= (j - i) * K;
    row2_stripe >>= (j - i) * K;
    bits ^= row1_stripe;
    bits ^= row2_stripe;
  }
  void swap_col(int i, int j) {
    auto ones_stripe = 0;
    for (int i = 0; i < K; ++i) ones_stripe |= __uint128_t(1) << (i * K);
    auto row1_stripe = ones_stripe << (i);
    auto row2_stripe = ones_stripe << (j);
    row1_stripe &= bits;
    row2_stripe &= bits;
    bits ^= row1_stripe;
    bits ^= row2_stripe;
    row1_stripe <<= (j - i);
    row2_stripe >>= (j - i);
    bits ^= row1_stripe;
    bits ^= row2_stripe;
  }

  void print(std::ostream& output) {
    for (int j = 0; j < K; ++j) {
      for (int i = 0; i < K; ++i) {
        output << ((bits & (__uint128_t(1) << (j * K + i))) ? '1' : '0');
      }
      output << "\n";
    }
  }
  void read(std::istream& input) {
    bits = 0;
    for (int j = 0; j < K; ++j) {
      for (int i = 0; i < K; ++i) {
        char c;
        input >> c;
        if (c == '1') bits |= __uint128_t(1) << (j * K + i);
      }
    }
  }

  bool operator<(const Matrix m) const { return bits < m.bits; }

  __uint128_t bits;
};

std::map<int, int> K;

void Gen() {
  int k = 0;
  for (int i=0;i<(1<<10);++i) {
    std::bitset<10> b = i;
    int c = b.count();
    if (c >=4 && c <=6) continue;
    if (b[4] != b[5] || b[7] != b[8]) continue;
    K[k] = i;
    ++k;
  }
  std::clog << "word size: " << k << std::endl;
}

void Gen2(int ones) {
  int k = 0;
  for (int i=0;i<(1<<10);++i) {
    std::bitset<10> b = i;
    if (b.count() != ones) continue;
    // K[k] = i;
    ++k;
  }
  std::clog << "word size: " << k << std::endl;
}

int main() {
  std::ios_base::sync_with_stdio(0);

  Gen();
  for (int i=1;i<=10;++i) Gen2(i);
  // std::vector<Matrix<3>> M;
  // for (int i=0;i<(1<<9);++i) {
  //   Matrix<3> m;
  //   m.bits = i;
  //   M.push_back(m);
  // }
//   std::sort(M.begin(), M.end());
  

  // int i = 0;
  // for (auto m : M) {
  //   std::cout << i << " :: " << std::endl;
  //   m.print(std::cout);
  //   std::cout << std::endl;
  //   ++i;
  // }

  std::string role;
  std::cin >> role;
  int t;
  long n;
  std::cin >> n >> t;
  if (role == "Algosia") {
    while (t--) {
      long v;
      std::cin >> v;

    }
  } else if (role == "Bajtek") {
  } else {
    std::cerr << "unsupported role" << std::endl;
  }
  return 0;
}