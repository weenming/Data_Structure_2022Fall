#include <iostream>
#include <vector>

using namespace std;

/**
 * if maze[i][j] == 0: unfeasible path, else feasible
 * along diagonal not allowed
 */
vector<vector<int>> constructMaze(int n) {
    vector<vector<int>> maze(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j || i == j - 1) maze[i][j] = 1;
        }
    }
    return maze;
}

vector<vector<int>> constructMaze2() {
    int n = 5;
    vector<vector<int>> maze(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j - 2) maze[i][j] = 1;
        }
    }
    maze[0][0] = maze[4][4] = 1;
    maze[0][1] = 1;
    maze[0][3] = 1;
    maze[2][4] = 1;
    maze[3][4] = 1;
    maze[1][4] = 1;
    return maze;
}

/**
 * i: index of row
 * j: index of col
 */
vector<vector<int>> findPath(vector<vector<int>>* maze, int i, int j) {
    int n = maze->size();
    // exit recur
    if (i == n - 1 && j == n - 1)
        return vector<vector<int>>(1, vector<int>({i, j}));
    (*maze)[i][j] = 0;
    // if blocked, return empty vector so that caller knows
    bool north = j + 1 < n && (*maze)[i][j + 1];
    bool south = j - 1 > -1 && (*maze)[i][j - 1];
    bool east = i + 1 < n && (*maze)[i + 1][j];
    bool west = i - 1 > -1 && (*maze)[i - 1][j];

    bool all_blocked = !(north || south || east || west);
    // using pointer to access [] op:
    // https://stackoverflow.com/questions/8493829/accessing-the-operator-from-a-pointer
    if (all_blocked) return vector<vector<int>>();
    if (east) {
        auto east_path = findPath(maze, i + 1, j);
        if (east_path.size() != 0) {
            east_path.insert(east_path.begin(), vector<int>({i, j}));
            return east_path;
        }
    }
    if (west) {
        auto west_path = findPath(maze, i - 1, j);
        if (west_path.size() != 0) {
            west_path.insert(west_path.begin(), vector<int>({i, j}));
            return west_path;
        }
    }
    if (north) {
        auto north_path = findPath(maze, i, j + 1);
        if (north_path.size() != 0) {
            north_path.insert(north_path.begin(), vector<int>({i, j}));
            return north_path;
        }
    }
    if (south) {
        auto south_path = findPath(maze, i, j - 1);
        if (south_path.size() != 0) {
            south_path.insert(south_path.begin(), vector<int>({i, j}));
            return south_path;
        }
    }
    return vector<vector<int>>();
}

void test() {
    int n = 10;
    auto maze = constructMaze2();
    // print maze
    cout << "maze: " << endl;
    for (int i = 0; i < maze.size(); i++) {
        auto this_row = maze[i];
        for (int j = 0; j < this_row.size(); j++) {
            cout << this_row[j] << ' ';
        }
        cout << endl;
    }
    cout << endl;

    auto path = findPath(&maze, 0, 0);

    // print path:
    cout << "path: " << endl;
    for (int i = 0; i < path.size(); i++) {
        auto this_row = path[i];
        for (int j = 0; j < this_row.size(); j++) {
            cout << this_row[j] << ' ';
        }
        cout << endl;
    }
    cout << endl;

    return;
}

int main() {
    test();
    return 0;
}