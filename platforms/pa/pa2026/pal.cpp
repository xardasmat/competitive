#include <iostream>

// #define DEBUG

#ifdef DEBUG
int max_pal(const std::string& S) {
  std::string s = "x" + S + "D";
  int max = 0;
  for (int i = 1; i < s.length() - 1; ++i) {
    int t = 1;
    while (s[i - t] == s[i + t]) ++t;
    max = std::max(2 * t - 1, max);

    t = 1;
    while (s[i - t + 1] == s[i + t]) ++t;
    max = std::max(2 * t - 2, max);
  }

  return max;
}

std::string to_str(long x, int bits) {
  std::string s;
  for (int i = 0; i < bits; ++i) s += (x & (1ll << i)) ? 'A' : 'P';
  return s;
}
#endif

std::string genpal(int n, int k) {
  if (k < 4) return "NIE";
  std::string pal = std::string(k, 'P');
  while (pal.size() < n) pal += "APAAPP";
  while (pal.size() > n) pal.pop_back();
#ifdef DEBUG
  if (max_pal(pal) != k)
    std::clog << "Verify: " << max_pal(pal) << " vs " << k << std::endl;
#endif
  return pal;
}

int len[] = {1, 1, 2, 2, 3, 3, 3, 3};
std::string str[3][8] = {
    {"A", "AP", "NIE", "NIE", "NIE", "NIE", "NIE", "NIE"},
    {"NIE", "AA", "AAP", "AAPP", "NIE", "NIE", "NIE", "NIE"},
    {"NIE", "NIE", "AAA", "AAAP", "PAPPP", "APAPPP", "AAPAPPP", "AAAPAPPP"}};

void solve() {
  int n, k;
  std::cin >> n >> k;
  if (n > 8 || k >= 4)
    std::cout << genpal(n, k) << std::endl;
  else {
#ifdef DEBUG
    if (str[k - 1][n - 1] != "NIE" && max_pal(str[k - 1][n - 1]) != k)
      std::clog << "Verify: " << max_pal(str[k - 1][n - 1]) << " vs " << k
                << std::endl;
#endif
    std::cout << str[k - 1][n - 1] << std::endl;
  }
}

int main() {
  std::ios_base::sync_with_stdio(0);

#ifdef DEBUG
  for (long i = 1; i < 20; ++i) {
    int best = i + 1;
    std::string best_str;
    for (long x = 0; x < (1ll << i); ++x) {
      auto s = to_str(x, i);
      int pal = max_pal(s);
      if (pal < best) {
        best = pal;
        best_str = s;
      }
    }
    std::cout << "len: " << i << "; pal: " << best << "; str: " << best_str
              << std::endl;
  }
#endif

  int t;
  std::cin >> t;
  while (t--) solve();

  return 0;
}