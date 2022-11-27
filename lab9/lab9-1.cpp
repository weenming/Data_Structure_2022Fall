#include <iostream>
#include <stack>
#include <vector>

#include "vector2tree.hpp"
using namespace std;

void swap(TreeNode* root, TreeNode* err1, TreeNode* err2, int err1_val,
          int err2_val) {
    if (!root) return;

    if (root == err1)
        root->val = err2_val;
    else if (root == err2)
        root->val = err1_val;

    swap(root->left, err1, err2, err1_val, err2_val);
    swap(root->right, err1, err2, err1_val, err2_val);
    return;
}

void mySolution(TreeNode* root) {
    // 时间复杂是O(N)
    // 空间复杂度是O(N)
    stack<TreeNode*> s;
    TreeNode *cur = root, *prev_node, *err_node_1, *err_node_2;
    int prev_val = 0;
    s.push(root);
    while (!s.empty()) {
        while (cur->left) {
            s.push(cur->left);
            cur = cur->left;
        }
        cur = s.top();
        // visit
        // cout << cur->val << ' ';
        if (cur->val < prev_val && cur->val) {
            break;
        }
        if (cur->val) {  // exclude 0 (NULL)
            prev_val = cur->val;
            prev_node = cur;
        }
        s.pop();
        if (cur->right) {
            s.push(cur->right);
            cur = cur->right;
        }
    }
    err_node_1 = prev_node;
    // cout << endl;
    // 反向中序遍历 从大到小
    while (!s.empty()) s.pop();
    s.push(root);
    cur = root;
    prev_val = 0;
    while (!s.empty()) {
        while (cur->right) {
            s.push(cur->right);
            cur = cur->right;
        }
        cur = s.top();
        // visit
        // cout << cur->val << ' ';
        if (cur->val > prev_val && prev_val && cur->val) {
            break;
        }
        if (cur->val) {  // exclude 0 (NULL)
            prev_val = cur->val;
            prev_node = cur;
        }
        s.pop();
        if (cur->left) {
            s.push(cur->left);
            cur = cur->left;
        }
    }
    err_node_2 = prev_node;
    // cout << endl;
    cout << "swap " << err_node_1->val << ',' << err_node_2->val << endl;
    swap(root, err_node_1, err_node_2, err_node_1->val, err_node_2->val);
    return;
}

void test() {
    cout << "before:" << endl;

    // test case 1
    // vector<int> a = {1, 3, NULL, NULL, 2};

    // test case 2: swap 2 and 3
    // vector<int> a = {4, 3, 5, 1, 2};

    // test case 3: swap 2 and 4, separated by NULL
    // vector<int> a = {2, 4, 6, 1, NULL, 5, 7};

    // test case 4: not neighboring
    vector<int> a = {4, 2, 6, 1, 7, 5, 3};

    for (int i = 0; i < a.size(); i++) cout << a[i] << ' ';
    cout << endl;
    cout << "before" << endl;
    TreeNode* root = createTree(a, 0);
    printTree(get_vec_tree(root));

    mySolution(root);
    cout << "after" << endl;
    printTree(get_vec_tree(root));
}

int main() {
    cout << "test big tree" << endl;
    vector<int> tree_vec(127, 1);
    printTree(tree_vec);
    cout << "test small tree" << endl;
    vector<int> tree_vec_2(15, 1);
    printTree(tree_vec_2);
    test();
    return 0;
}