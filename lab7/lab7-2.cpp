#include <iostream>

#include "myStack.hpp"
// #include"myQueue.hpp"
#include <queue>
#include <stack>
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

void non_recursive_flatten(TreeNode* root) {
    // 时间复杂度 O(N)
    // 空间复杂度 O((N)), (left_nodes最大空间也是深度) N是总节点数
    stack<TreeNode*> left_nodes;
    queue<TreeNode*> tree;
    TreeNode* cur = root;
    // 顺序访问 (入队)：根->左->右
    left_nodes.push(root);
    while (!left_nodes.empty()) {
        cur = left_nodes.top();
        left_nodes.pop();
        while (cur) {
            tree.push(cur);  // 访问过这个节点
            // cout << cur->value << endl;
            // 遍历左边， 右边非空子树根节点入栈
            if (cur->right) left_nodes.push(cur->right);
            cur = cur->left;  // 换到最左下的未经过的节点遍历左边
        }
    }
    cur = root;
    while (!tree.empty()) {
        // cout << cur << " right " << cur->right << " value " << cur->value
        //      << endl;
        cur->left = nullptr;
        cur->right = tree.front();
        tree.pop();
        cur = cur->right;
    }
    cur->left = nullptr;
    cur->right = nullptr;
}

TreeNode* recursive_flatten(TreeNode* root) {
    // 时间复杂度 O(N)
    // 空间复杂度 O(log2 (N)), N 是总节点数
    TreeNode *left_end = nullptr, *right_end = nullptr;
    if (root != nullptr) {
        left_end = recursive_flatten(root->left);
        right_end = recursive_flatten(root->right);

        if (left_end && right_end) {
            // -> (root->left) -> ... -> left_end -> (root->right) -> ... ->
            // right_end (returned) ->
            left_end->right = root->right;
            root->right = root->left;
            left_end->left = nullptr;
        } else if (left_end && !right_end) {
            // -> (root->left) -> ... -> left_end (returned) ->
            // right_end 是最后一个节点的指针（不是它的right指针）所以不能是空的
            left_end->right = root->right;
            root->right = root->left;
            left_end->left = nullptr;
            right_end = left_end;
        } else if (right_end) {
            // -> root -> (root->right) -> ... -> right_end (returned) ->
            // root->right = right_end;
            right_end->left = nullptr;
        } else {
            // -> root (returned)->
            right_end = root;
        }
        root->left = nullptr;
    }
    // if (!right_end && root) {
    //     return root;
    // }
    return right_end;
}

void mySolution(TreeNode* root) {
    // 时间复杂度：O(N)
    // 空间复杂度：O(1)
    // 我不知道怎么在函数里新初始化一个东西传递到函数外面，就是如果不是存在函数的栈帧上，那函数返回的时候会自动析构吗
    // 不过这里好像没关系了。。。
    recursive_flatten(root);
    // non_recursive_flatten(root);
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

    TreeNode* cur = root;

    mySolution(root);
    while (cur) {
        cout << cur->value << " ";
        cur = cur->right;
    }
    cout << endl;
    return;
}

int main() {
    test();
    return 0;
}