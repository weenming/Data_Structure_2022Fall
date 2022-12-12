#include <iostream>
#include <vector>

#include "Graph.cpp"
using namespace std;

int find_min(vector<int>& vec, vector<bool>& not_applicable) {
    int min = -1;
    int min_index = -1;
    for (int i = 1; i < vec.size(); i++) {
        if (!not_applicable[i] && (min > vec[i] || min < 0)) {
            min = vec[i];
            min_index = i;
        }
    }
    return min_index;
}

// Manhattan distance
int md(vector<int> r1, vector<int> r2) {
    return abs(r1[0] - r2[0]) + abs(r1[1] - r2[1]);
}

int mySolution(vector<vector<int>>& arr) {
    /*用 Prim 算法，比 Kruskal 算法快
      时间复杂度: O(V^2)
      空间复杂度: O(V)
    */
    int V = arr.size();
    vector<int> lowest_distance = vector(V, 0);
    vector<bool> in_spanning_tree = vector(V, true);
    for (int i = 1; i < V; i++) {
        lowest_distance[i] = md(arr[0], arr[i]);
        in_spanning_tree[i] = false;
    }
    for (int i = 1; i < V; i++) {
        int min_index = find_min(lowest_distance, in_spanning_tree);
        in_spanning_tree[min_index] = true;
        for (int j = 0; j < V; j++) {
            int new_distance = md(arr[min_index], arr[j]);
            if (!in_spanning_tree[j] && new_distance < lowest_distance[j]) {
                lowest_distance[j] = new_distance;
            }
        }  // end update lowest_distance
    }      // all vertexes added to spanning tree
    int sum = 0;
    for (int d : lowest_distance) sum += d;
    return sum;
}

void test1() {
    vector<vector<int>> pts = {{0, 0}, {2, 2}, {3, 10}, {5, 2}, {7, 0}};
    cout << mySolution(pts) << endl;
}

void test2() {
    vector<vector<int>> pts = {{0, 0}, {1, 1}, {0, 1}, {1, 0}};
    cout << mySolution(pts) << endl;
}

void test3() {
    vector<vector<int>> pts = {{0, 0}, {1, 1}};
    cout << mySolution(pts) << endl;
}

void test4() {
    vector<vector<int>> pts = {{0, 0}};
    cout << mySolution(pts) << endl;
}

int main() {
    test1();
    test2();
    test3();
    test4();

    return 0;
}