#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

int n,m;
std::vector<std::string> M2;
std::vector<std::string> M;

std::pair<int,int> DV[8] = {
    {-1, -1},{0, -1}, {1, -1},
    {-1,  0},         {1,  0},
    {-1,  1},{0,  1}, {1,  1}
};

bool ok(int x, int y) {
    return x >= 0 && y >= 0 && x < m && y < n;
}

bool stable(int x, int y) {
    for (const auto [dx, dy] : DV)
        if (ok(x+dx, y+dy) && M[y+dy][x+dx] != M[y][x])
            return true;
    return false;
}

char convert(char c, int cycles) {
    if (c == 2) return '.';
    if (cycles == 0 && c == 0) return '.';
    if (cycles == 0 && c == 1) return '#';
    if (cycles == 1 && c == 0) return '#';
    if (cycles == 1 && c == 1) return '.';
    return c+'0';
}

int main() {
    std::ios_base::sync_with_stdio(0);
    std::cin >> n >> m;
    M.resize(n);
    for (auto &R : M) std::cin >> R;
    M2 = M;

    std::queue<std::pair<int,int>> Q;
    for (int i=0;i<n;++i){
        for (int j=0;j<m;++j){
            if (!stable(j, i))
                M2[i][j] = 2;
            else if (M[i][j] == '.') {
                M2[i][j] = 1;
                Q.push({j,i});
            }
            else {
                M2[i][j] = 0;
                Q.push({j,i});
            }
        }
    }
    int cycles = 1;
    std::swap(M, M2);
    
    std::queue<std::pair<int,int>> Q2;
    while (!Q.empty() || !Q2.empty()) {
        if (Q.empty()) {
            std::swap(Q, Q2);
            ++cycles;
        }
        auto [x, y] = Q.front();
        Q.pop();

        for (const auto [dx, dy] : DV){
            if (ok(x+dx, y+dy) && M[y+dy][x+dx] == 2) {
                M[y+dy][x+dx] = !M[y][x];
                Q2.push({x+dx, y+dy});
            }
        }
    }

    for (int i=0;i<n;++i)
        for (int j=0;j<m;++j)
            M[i][j] = convert(M[i][j], 1);
    
    for (auto &R : M) std::cout << R << std::endl;
    return 0;
}