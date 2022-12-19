#include <iostream>
#include <vector>

using namespace std;

vector<int> Solution(int n, vector<vector<int> >& matrix) {
    // 用拓扑排序，维护一个长度为n 的直接前驱计数
    vector<int> pre_count = vector<int>(n, 0);
    int edges = matrix.size();
    vector<int> res = vector<int>();

    // 初始化pre_count
    for (int i = 0; i < edges; i++) {
        pre_count[matrix[i][0]] += 1;
    }

    // 找到源点
    int src_index = -1;
    for (int i = 0; i < n; i++) {
        if (pre_count[i] == 0) {
            pre_count[i] = src_index;
            src_index = i;
        }
    }
    // 开始排序
    while (src_index + 1) {
        // 当前源点由src_index指明
        res.push_back(src_index);

        int last_src_index = src_index;
        // 更新src_index
        src_index = pre_count[src_index];

        // 把src的出边删掉，更新pre_count和src表
        for (int i = 0; i < edges; i++) {
            int node_i = matrix[i][0];
            // 如果入边正好是刚刚删掉的出边，就更新计数
            pre_count[node_i] -= (matrix[i][1] == last_src_index);
            if (!pre_count[node_i]) {
                pre_count[node_i] = src_index;
                src_index = node_i;
            }
        }
        // 说明没有入度为0的vertex了
        if (!(src_index + 1)) {
            if (res.size() == n) break;
            // 否则没全加进去，说明不存在
            return vector<int>();
        }
    }
    return res;
}

void test1() {
    int n = 2;

    vector<int> vi(2, 0);
    vi[0] = 1;
    vi[1] = 0;
    vector<vector<int> > matrix;
    matrix.push_back(vi);

    vector<int> ans = Solution(n, matrix);
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << " ";
    }
    cout << endl;

    return;
}

void test2() {
    int n = 4;

    vector<int> v0 = {1, 0};
    vector<int> v1 = {2, 0};
    vector<int> v2 = {3, 1};
    vector<int> v3 = {3, 2};

    vector<vector<int> > matrix;
    matrix.push_back(v0);
    matrix.push_back(v1);
    matrix.push_back(v2);
    matrix.push_back(v3);

    vector<int> ans = Solution(n, matrix);
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << " ";
    }
    cout << endl;

    return;
}

void test3() {
    // 不存在
    int n = 4;

    vector<int> v0 = {1, 0};
    vector<int> v1 = {2, 0};
    vector<int> v2 = {3, 1};
    vector<int> v3 = {0, 3};

    vector<vector<int> > matrix;
    matrix.push_back(v0);
    matrix.push_back(v1);
    matrix.push_back(v2);
    matrix.push_back(v3);

    vector<int> ans = Solution(n, matrix);
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << " ";
    }
    cout << endl;

    return;
}

void test4() {
    // matrix.size()和n不一样，应该是
    // 0, 2, 4, 1, 3
    int n = 5;

    vector<int> v0 = {1, 0};
    vector<int> v1 = {2, 0};
    vector<int> v2 = {1, 2};
    vector<int> v3 = {1, 4};
    vector<int> v4 = {4, 2};
    vector<int> v5 = {3, 1};

    vector<vector<int> > matrix;
    matrix.push_back(v0);
    matrix.push_back(v1);
    matrix.push_back(v2);
    matrix.push_back(v3);
    matrix.push_back(v4);
    matrix.push_back(v5);

    vector<int> ans = Solution(n, matrix);
    for (int i = 0; i < ans.size(); ++i) {
        cout << ans[i] << " ";
    }
    cout << endl;

    return;
}

int main() {
    test4();

    return 0;
}