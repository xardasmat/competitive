// #define DEBUG
#include <iostream>
#include <map>
#include <numeric>
#include <stack>
#ifdef DEBUG
#include <iomanip>
#endif

const int MAX = 1000005;
const int MAXK = 20;

int A[MAX * 2];
// int N[MAX * 2][MAXK];

// int solve(int i, int n, int k) {
//   if (i >= n) return 0;
//   if (k < 0) return 0;
//   int ni = N[i][k];
//   if (ni < n)
//     return solve(ni, n, k - 1) + (1 << k);
//   else
//     return solve(i, n, k - 1);
// }

int main() {
  std::ios_base::sync_with_stdio(0);
  int n;
  std::cin >> n;
  for (int i = 0; i < n; ++i) std::cin >> A[i];
  for (int i = 0; i < n; ++i) A[i + n] = A[i];
  int max = A[0];
  for (int i = 1; i < n; ++i) max = std::max(max, A[i]);
  // n log n solution
  // std::map<int, int> M;
  // A[2 * n] = MAX;
  // M[MAX] = 2 * n;
  // for (int i = 0; i < MAXK; ++i) N[2*n][i] = 2*n;
  // for (int i = 2 * n - 1; i >= 0; --i) {
  //   while (M.begin()->first <= A[i]) M.erase(M.begin());
  //   N[i][0] = M.begin()->second;
  //   for (int k = 1; k < MAXK; ++k) N[i][k] = N[N[i][k - 1]][k - 1];
  //   M[A[i]] = i;
  // }
#ifdef DEBUG
  for (int i = 0; i <= 2 * n; ++i) {
    std::clog << std::setw(4) << i;
  }
  std::clog << std::endl;
  for (int i = 0; i <= 2 * n; ++i) {
    std::clog << std::setw(4) << (A[i] < MAX ? A[i] : -1);
  }
  std::clog << std::endl;
  for (int i = 0; i <= 2 * n; ++i) {
    std::clog << std::setw(4) << N[i][0];
  }
  std::clog << std::endl;
#endif
  int best = 0;
  int start = 2 * n - 1;
  while (A[start] < max) --start;
  std::stack<int> S;
  int i = start - 1;
  int stop = start - n;
  S.push(max);
  while (i > stop) {
    while (!S.empty() && A[i] >= S.top()) S.pop();
    S.push(A[i]);
    --i;
    best = std::max(best, int(S.size()));
  }

  // for (int i = 0; i < n; ++i) {
  //   best = std::max(best, solve(i, i + n, MAXK - 1)+1);
  // }

  std::cout << best << std::endl;

  return 0;
}
