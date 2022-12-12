#include <vector>

using namespace std;
struct Node {
    Node* next;
    int index;
    int cost;
    Node(Node* ptr, int i, int c) : next(ptr), index(i), cost(c) {}
};

class LinkedList {
   private:
    Node* head;

   public:
    LinkedList() { head = new Node(nullptr, 0, 0); }
    void add(Node* new_node) {
        new_node->next = head->next;
        head->next = new_node;
    }
    Node* get_first() { return head->next; }
    Node* get_head() { return head; }
};

struct Vertex {
    LinkedList* adj_list;
    int index;
    Vertex(LinkedList* adj_list, int index)
        : adj_list(adj_list), index(index) {}
};

class Graph {
    vector<Vertex*> vertexes;

   public:
    Graph(vector<Vertex*> ls) : vertexes(ls) {}
    Graph(vector<vector<int>>);
    Graph(vector<vector<int>>, vector<vector<int>>);

    vector<vector<int>> DFS(int start, int dest);
    int size() { return vertexes.size(); }
};

vector<vector<int>> Graph::DFS(int start, int dest) {
    // No cycle in the graph
    LinkedList* list = vertexes[start]->adj_list;
    Node* cur = list->get_first();
    if (start == dest) {
        // return [[dest]]
        vector<vector<int>> res = vector(1, vector(1, dest));
        return res;
    }
    vector<vector<int>> res, tmp_res;
    while (cur) {
        tmp_res = DFS(cur->index, dest);
        for (int i = 0; i < tmp_res.size(); i++) {
            vector<int> route = tmp_res[i];
            route.insert(route.begin(), start);
            tmp_res[i] = route;
        }
        // res = [[route1], [route2], [route3], ...]
        res.insert(res.begin(), tmp_res.begin(), tmp_res.end());
        cur = cur->next;
    }
    return res;
}

Graph::Graph(vector<vector<int>> adj_list) {
    for (int i = 0; i < adj_list.size(); i++) {
        LinkedList* list = new LinkedList();
        for (int j : adj_list[i]) {
            Node* node = new Node(nullptr, j, 1);
            list->add(node);
        }
        Vertex* vertex = new Vertex(list, i);
        vertexes.insert(vertexes.begin() + i, vertex);
    }
}

Graph::Graph(vector<vector<int>> adj_list, vector<vector<int>> cost) {
    for (int i = 0; i < adj_list.size(); i++) {
        LinkedList* list = new LinkedList();
        for (int j = 0; j < adj_list[i].size(); j++) {
            Node* node = new Node(nullptr, adj_list[i][j], cost[i][j]);
            list->add(node);
        }
        Vertex* vertex = new Vertex(list, i);
        vertexes.insert(vertexes.begin() + i, vertex);
    }
}