#include <iostream>
#include <algorithm>
#include <vector>

const int MAX = 500005;

std::vector<int> G[MAX];
std::vector<int> C[MAX];

int main() {
    std::ios_base::sync_with_stdio(0);
    long k, n;
    std::cin >> k >> n;

    long max_total = n;
    for (int j=0;j<n;++j) {
        G[0].push_back(0);
        C[0].push_back(0);
    }

    for (int i=1;i<k;++i) {
        std::cin >> n;
        for (int j=0;j<n;++j) {
            int a;
            std::cin >> a;
            G[i].push_back(a);
            C[i].push_back(i == (k-1) ? 1 : 0);
        }
    }
    for (int i=k-1;i>=0;--i) {
        long total = 0;
        for (int j=0;j<G[i].size();++j) {
            if (i > 0 && G[i][j] > 0) {
                C[i-1][G[i][j]-1] += std::max(C[i][j], 1);
            }
            total += std::max(C[i][j], 1);
        }
        max_total = std::max(max_total, total);
    }

    std::cout << max_total << std::endl;

    return 0;
}
