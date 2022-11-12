#include <iostream>

#include "myStack.hpp"
// #include"myQueue.hpp"
#include <queue>
#include <vector>
using namespace std;

typedef struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode() : value(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : value(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode* right)
        : value(x), left(left), right(right) {}
} TreeNode;

vector<vector<int>> mySolution(TreeNode* root) {
    // 时间复杂度：O(N)
    // 空间复杂度：O(N), where N is number of nodes
    // Iterate from right to left, from top to bottom
    queue<TreeNode*> node_queue;
    Stack<vector<int>> tree_stack;
    vector<vector<int>> tree_vec;
    vector<int> layer_vec;
    TreeNode* last_layer_node = NULL;
    TreeNode* node;
    node_queue.push(root);
    // When dynamically allocating space for vector,
    //  iterators do not function correctly?
    while (!node_queue.empty()) {
        node = node_queue.front();
        // No return value!!
        node_queue.pop();
        // Use stack to store vector of layers, but vector to store nodes in one
        // layer. Therefore nodes in one layer is not reversed when popping from
        // stack (vector copy).
        if (node->left) {
            node_queue.push(node->left);
            // if last_layer_node is NULL, set the most left node
            if (!last_layer_node) {
                last_layer_node = node->left;
            }
        }
        if (node->right) {
            node_queue.push(node->right);
            if (!last_layer_node) {
                last_layer_node = node->right;
            }
        }

        if (node == last_layer_node) {
            // Now node is at the next layer
            // maintain `last layer node` to indicate layer in the tree
            // copy of layer_vec
            tree_vec.insert(tree_vec.begin(), layer_vec);
            layer_vec.clear();
            layer_vec.resize(0);
            // if this node (the leftest of the current layer is not NULL, set
            // this node, otherwise set the existing leftest node)
            if (node->left) {
                last_layer_node = node->left;
            } else if (node->right) {
                last_layer_node = node->right;
            } else {
                last_layer_node = NULL;
            }
        }
        layer_vec.push_back(node->value);
    }
    // break if queue is empty, so last layer is not saved
    // Use vector insert instead of stack...
    tree_vec.insert(tree_vec.begin(), layer_vec);

    // // transfer to vector
    // while (!tree_stack.is_empty()) {
    //     tree_vec.push_back(tree_stack.pop());
    // }
    return tree_vec;
}

void print_tree(TreeNode* root, string indent) {
    TreeNode* p = root;
    // 前序遍历
    if (p != nullptr) {
        cout << endl << indent << p->value;
        indent += "  ";
        print_tree(p->left, indent);
        print_tree(p->right, indent);
    }
}

void print_vector2(vector<vector<int>> res) {
    cout << '[' << endl;
    for (auto i : res) {
        cout << '[';
        for (auto c : i) {
            cout << c << " ";
        }
        cout << ']' << endl;
    }
    cout << ']';
    cout << endl;
}

void test() {
    TreeNode* Node_1 = new TreeNode(1);
    TreeNode* Node_3 = new TreeNode(3);
    TreeNode* Node_4 = new TreeNode(4);
    TreeNode* Node_2 = new TreeNode(2, Node_3, Node_4);
    TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // TreeNode* Node_5 = new TreeNode(5);
    // TreeNode* Node_6 = new TreeNode(6);
    // TreeNode* Node_3 = new TreeNode(3);
    // TreeNode* Node_4 = new TreeNode(4);
    // TreeNode* Node_2 = new TreeNode(2, Node_3, Node_4);
    // TreeNode* Node_1 = new TreeNode(1, Node_5, Node_6);
    // TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // TreeNode* Node_5 = new TreeNode(5);
    // TreeNode* Node_6 = new TreeNode(6);
    // TreeNode* Node_2 = new TreeNode(2);
    // TreeNode* Node_1 = new TreeNode(1, Node_5, Node_6);
    // TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // TreeNode* Node_9 = new TreeNode(9);
    // TreeNode* Node_10 = new TreeNode(10);
    // TreeNode* Node_7 = new TreeNode(7);
    // TreeNode* Node_8 = new TreeNode(8);
    // TreeNode* Node_5 = new TreeNode(5);
    // TreeNode* Node_6 = new TreeNode(6);
    // TreeNode* Node_3 = new TreeNode(3, Node_7, Node_8);
    // TreeNode* Node_4 = new TreeNode(4, Node_9, Node_10);
    // TreeNode* Node_2 = new TreeNode(2, Node_5, Node_6);
    // TreeNode* Node_1 = new TreeNode(1, Node_3, Node_4);
    // TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // TreeNode* Node_2 = new TreeNode(2);
    // TreeNode* Node_1 = new TreeNode(1);
    // TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // TreeNode* root = new TreeNode(0);

    // TreeNode* Node_7 = new TreeNode(7);
    // TreeNode* Node_8 = new TreeNode(8);
    // TreeNode* Node_5 = new TreeNode(5);
    // TreeNode* Node_6 = new TreeNode(6);
    // TreeNode* Node_3 = new TreeNode(3, Node_7, Node_8);
    // TreeNode* Node_4 = new TreeNode(4);
    // TreeNode* Node_2 = new TreeNode(2, nullptr, Node_4);
    // TreeNode* Node_1 = new TreeNode(1, Node_5, nullptr);
    // TreeNode* root = new TreeNode(0, Node_1, Node_2);

    // // right nodes
    // TreeNode* Node_4 = new TreeNode(4);
    // TreeNode* Node_3 = new TreeNode(3, nullptr, Node_4);
    // TreeNode* Node_2 = new TreeNode(2, nullptr, Node_3);
    // TreeNode* root = new TreeNode(0, nullptr, Node_2);

    // // left nodes
    // TreeNode* Node_4 = new TreeNode(4);
    // TreeNode* Node_3 = new TreeNode(3, Node_4, nullptr);
    // TreeNode* Node_2 = new TreeNode(2, Node_3, nullptr);
    // TreeNode* root = new TreeNode(0, nullptr, Node_2);

    vector<vector<int>> res = mySolution(root);
    print_vector2(res);
    return;
}

void test_big_tree() {
    // complete binary tree
    // construct with double ptr
    TreeNode empty_node(0, nullptr, nullptr);
    vector<TreeNode> node_vec(16, empty_node);
    vector<TreeNode>::iterator i = node_vec.begin();
    auto last_i = i;
    i++;
    int begin_add = int(&*last_i);
    for (auto i = node_vec.begin(); i != node_vec.end(); i++) {
        i->value = int(&*i) - begin_add;
    }
    while (1) {
        last_i->left = &(*(i++));
        if (i == node_vec.end()) break;
        last_i->right = &(*(i++));
        if (i == node_vec.end()) break;
        last_i++;
    }
    TreeNode* root = &(node_vec[0]);
    // print_tree(root, "");
    auto res = mySolution(root);
    print_vector2(res);
}

int main() {
    test();
    return 0;
}