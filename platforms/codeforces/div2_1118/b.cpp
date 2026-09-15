#include <iostream>
#include <algorithm>
#include <vector>

const int MAX = 200001;
int64_t A[MAX];
int64_t DD[MAX];
int64_t DDp[MAX];

//1 2 3 4 4


void solve() {
    int64_t n, m;
    std::cin >> n >> m;
    for (int i=0;i<n;++i) std::cin >> A[i];
    std::sort(A, A+n);
    int64_t sum = 0;
    for (int i=0;i<n;++i) sum += A[i];
    
    int64_t* D = DD;
    int64_t* Dp = DDp;

    int64_t max = 0;
    for (int k=1;k<=m;++k) {
        int64_t ll = std::distance(A, std::lower_bound(A, A+n, k));
        int64_t l = std::distance(A, std::lower_bound(A, A+n, 2*k));
        int64_t r = std::distance(A, std::upper_bound(A, A+n, 2*k));
        D[k] = n-ll;
        max = std::max(max, D[k] + (r-l));
        
    }
    std::cout << max;
    
    int64_t pow = 2;
    int jj = 2;
    for (int j=2;j<=m;++j) {
        pow *= 2;
        pow = std::min(pow, m+1);
        if (pow < m) {
            max = 0;
            for (;jj<pow;++jj) {
                std::swap(D, Dp);
                for (int k=1;jj*k<=m;++k) {
                    // D[j][k] = D[j-1] + count( >= jk ) [+ count( == (j+1)k)]
                    // solve(j) = max_k(d[j][k])
                    int64_t ll = std::distance(A, std::lower_bound(A, A+n, jj*k));
                    int64_t l = std::distance(A, std::lower_bound(A, A+n, (jj+1)*k));
                    int64_t r = std::distance(A, std::upper_bound(A, A+n, (jj+1)*k));
                    D[k] = Dp[k] + (n-ll);
                    if (jj == pow-1) max = std::max(max, D[k] + (r-l));
                }
            }
        } else { max = sum; }
        std::cout << " " << max;
    }
    std::cout << std::endl;
}

int main(int argc, char** argv) {
#ifdef UT
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
    std::ios_base::sync_with_stdio(0);
    int t;
    std::cin >> t;
    while (t--) solve();
    return 0;
}
