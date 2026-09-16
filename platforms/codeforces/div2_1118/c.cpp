#include <iostream>
#include <vector>
#ifdef UT
#include <gtest/gtest.h>
#endif

const int MAX = 1000;

#ifdef INTERACTOR
#include <fstream>
std::vector<int> V[MAX+1];
int D[MAX+1][MAX+1];
void dfs(const int v, const int d, const int a) {
    D[a][v] = d;
    for (int u : V[v]) if (D[a][u] < 0) dfs(u, d+1, a);
}
bool interact(std::istream& test) {
    int n;
    test >> n;
    std::clog << "DBG test case n=" << n << std::endl;
    std::cout << n << std::endl;
    for (int i=1;i<=n;++i) V[i].clear();
    for (int i=1;i<n;++i) {
        int a,b;
        test >> a >> b;
        V[a].push_back(b);
        V[b].push_back(a);
    }

    for (int i=1;i<=n;++i)
        for (int j=1;j<=n;++j)
            D[i][j] = -1;

    for (int i=1;i<=n;++i)
        dfs(i, 0, i);
    
    int max = 0;
    for (int i=1;i<=n;++i)
        for (int j=i+1;j<=n;++j)
            max = std::max(max, D[i][j]);


    std::clog << "DBG solution is " << max << std::endl;

    int count  = 0;
    bool failed = false;
    while (true) {
        char c;
        int a,b,d;
        std::cin >> c >> a >> b >> d;
        if (c == '!') {

            std::clog << "DBG solution answered = (" << a << " " << b << " " << d << ")" << std::endl;
            if (D[a][b] == d && d == max) return false || failed;
            else return true;
        }
        else if (c == '?') {
            ++count;
            if (count > 3*n) {
                std::clog << "DBG solution exceeded the limit of queries: " << count << " vs max = " << 3*n << std::endl;
                failed = true;
            }
            std::clog << "DBG solution asked = (" << a << " " << b << " " << d << "), answer is " << (D[a][b] >= d ? 1 : 0) << std::endl;
            std::cout << (D[a][b] >= d ? 1 : 0) << std::endl;
        } 
    }
    return true;
}

int interact_cases(std::istream& test) {
    int t;
    test >> t;
    std::cout << t << std::endl;
    bool failed = false;
    while (t--) failed |= interact(test);
    if (!failed) {
        std::cerr << "[SUCCESS]" << std::endl;
        return 0;
    } else {
        std::cerr << "[FAILED]" << std::endl;
        return 1;
    }
}
#endif

bool query(int a, int b, int d) {
    std::cout << "? " << a << " " << b << " " << d << std::endl;
    int res;
    std::cin >> res;
    return res == 1;
}

int get_len(int a,int b, int n) {
    int l = 1, r = n;
    while (l < r) {
        int m = (l+r+1)/2;
        if (query(a, b, m)) l = m;
        else r = m-1;
    }
    return l;
}

void solve() {
    int n;
    std::cin >> n;

    int max_len = 0;
    int a, b;
    for (int i=1;i<=n;++i)
        for (int j=i+1;j<=n;++j) {
            int len = get_len(i, j, n);
            if (len > max_len) {
                max_len = len;
                a = i;
                b = j;
            }
        }
    std::cout << "! " << a << " " << b << " " << max_len << std::endl;
}

int main(int argc, char** argv) {
    std::ios_base::sync_with_stdio(0);
#ifdef UT
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
#endif
#ifdef INTERACTOR
    if (argc > 1) {
        std::ofstream log_file("c.log");
        std::streambuf* original_clog_buffer = std::clog.rdbuf();
        std::clog.rdbuf(log_file.rdbuf());
        std::ifstream test(argv[1]);
        int result = interact_cases(test);
        std::clog.rdbuf(original_clog_buffer);
        return result;
    } else {
        std::cerr << "No input file specified" << std::endl;
        return 1;
    }
#endif
    int t;
    std::cin >> t;
    while (t--) solve();
    return 0;
}
