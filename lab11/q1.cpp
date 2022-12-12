#include <iostream>
#include <vector>

#include "Graph.cpp"

using namespace std;

vector<vector<int>> mySolution(vector<vector<int>>& adj_list) {
    /*
    Time complexity: O(V + E)
    Space complexity: O(D) (height of the call stack)
    where D is the depth of the graph, starting from Vertex 0
    The graph has V Vertexes and E edges
    */
    Graph* graph = new Graph(adj_list);
    return graph->DFS(0, graph->size() - 1);
}

void test1() {
    vector<vector<int>> adj_list = {{1, 2}, {3}, {3}, {}};
    vector<vector<int>> res = mySolution(adj_list);
    for (int i = 0; i < res.size(); i++) {
        cout << "route " << i << ": ";
        for (int j : res[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void test2() {
    vector<vector<int>> adj_list = {{1, 2, 3}, {4}, {5}, {5}, {6}, {6}, {}};
    vector<vector<int>> res = mySolution(adj_list);
    for (int i = 0; i < res.size(); i++) {
        cout << "route " << i << ": ";
        for (int j : res[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void test3() {
    vector<vector<int>> adj_list = {{1, 2}, {2}, {3, 4}, {4}, {}};
    vector<vector<int>> res = mySolution(adj_list);
    for (int i = 0; i < res.size(); i++) {
        cout << "route " << i << ": ";
        for (int j : res[i]) {
            cout << j << " ";
        }
        cout << endl;
    }
    cout << endl;
}

int main() {
    test1();
    test2();
    test3();
    return 0;
}