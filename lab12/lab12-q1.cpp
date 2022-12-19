#include <iostream>
#include <vector>

using namespace std;

int Solution(vector<vector<int>>& grid) {
    // 用Dijkstra算法
    int n = grid.size();
    // 最大长度不超过m^2
    int inf = n * n;
    // 初始化：用原矩阵存最小路径长度，如果是0表示这不是节点(原来是1的点)，负数表示没加进最小距离集合
    int tmp00 = grid[0][0] = 1;
    int tmp11 = -!grid[1][1] * 2;
    int tmp10 = -!grid[1][0] * 2;
    int tmp01 = -!grid[0][1] * 2;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (grid[i][j] == 0) {
                grid[i][j] = -inf;
            } else {
                // 原来是1
                grid[i][j] = 0;
            }
        }
    }
    grid[0][0] = tmp00;
    grid[1][0] = tmp10;
    grid[0][1] = tmp01;
    grid[1][1] = tmp11;

    // 开始寻找最短路径
    int best_i, best_j, best_d;
    while (true) {
        best_d = inf;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] < 0 && grid[i][j] > -best_d) {  // in set V - S
                    best_i = i;
                    best_j = j;
                    best_d = -grid[i][j];
                }
            }

        }  // 结束寻找 V-S 中最短路径的节点
        // 加入S中
        if (best_d != inf) grid[best_i][best_j] = best_d;
        // V-S中没有可以到达的点了，退出循环
        if (best_d == inf) break;
        // 更新V-S中的dist：只有相邻的
        for (int i = max(0, best_i - 1); i <= min(best_i + 1, n - 1); i++) {
            for (int j = max(0, best_j - 1); j <= min(best_j + 1, n - 1); j++) {
                if (grid[i][j] < 0 &&
                    best_d + 1 < -grid[i][j]) {  // 更新在 V-S 中的dist
                    grid[i][j] = -best_d - 1;    // 用负数表示
                }
            }
        }  // 结束更新dist
    }
    // 如果没找到，grid[n-1][n-1]应该是-inf
    if (grid[n - 1][n - 1] < 0) {
        return -1;
    } else {
        return grid[n - 1][n - 1];
    }
}

void test1() {
    vector<int> vi(2, 0);
    vi[0] = 0;
    vi[1] = 1;
    vector<vector<int>> grid;
    grid.push_back(vi);
    vi[0] = 1;
    vi[1] = 0;
    grid.push_back(vi);

    cout << Solution(grid) << endl;

    return;
}

void test2() {
    vector<int> v0 = {0, 0, 0};
    vector<int> v1 = {1, 1, 0};
    vector<int> v2 = {1, 1, 0};

    vector<vector<int>> grid;
    grid.push_back(v0);
    grid.push_back(v1);
    grid.push_back(v2);

    cout << Solution(grid) << endl;

    return;
}

void test3() {
    // Multiple paths, should return 3
    vector<int> v0 = {0, 0, 0};
    vector<int> v1 = {1, 0, 0};
    vector<int> v2 = {1, 1, 0};

    vector<vector<int>> grid;
    grid.push_back(v0);
    grid.push_back(v1);
    grid.push_back(v2);

    cout << Solution(grid) << endl;

    return;
}

void test4() {
    // Path does not exist: should return -1
    vector<int> v0 = {0, 0, 1};
    vector<int> v1 = {1, 1, 1};
    vector<int> v2 = {1, 1, 0};

    vector<vector<int>> grid;
    grid.push_back(v0);
    grid.push_back(v1);
    grid.push_back(v2);

    cout << Solution(grid) << endl;

    return;
}

void test5() {
    // Curvy path: should return
    vector<int> v0 = {0, 0, 0, 1};
    vector<int> v1 = {1, 1, 0, 1};
    vector<int> v2 = {1, 0, 1, 1};
    vector<int> v3 = {1, 0, 0, 0};

    vector<vector<int>> grid;
    grid.push_back(v0);
    grid.push_back(v1);
    grid.push_back(v2);
    grid.push_back(v3);

    cout << Solution(grid) << endl;

    return;
}

int main() {
    test5();

    return 0;
}