#include <iostream>
#include <queue>
#include <stack>
#include <vector>
using namespace std;

struct TreeNode {
    int val;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

// 实现创建二叉树的功能
TreeNode* createTree(vector<int> list, int start) {
    // 层序遍历的数组->二叉树
    if (list[start] == -1) {
        return NULL;
    }
    TreeNode* root = new TreeNode(list[start]);

    int lnode = 2 * start + 1;
    int rnode = 2 * start + 2;
    if (lnode > list.size() - 1) {
        root->left = NULL;
    } else {
        root->left = createTree(list, lnode);
        root->left->parent = root;
    }

    if (rnode > list.size() - 1) {
        root->right = NULL;
    } else {
        root->right = createTree(list, rnode);
        root->right->parent = root;
    }

    return root;
}
vector<int> get_vec_tree(TreeNode* root) {
    // 层序遍历
    queue<TreeNode*> q;
    q.push(root);
    vector<int> vec = {root->val};
    TreeNode* front;
    while (!q.empty()) {
        front = q.front();
        q.pop();

        if (front->left) {
            q.push(front->left);
            vec.insert(vec.end(), 1, front->left->val);
        } else {
            vec.insert(vec.end(), 1, 0);
        }

        if (front->right) {
            q.push(front->right);
            vec.insert(vec.end(), 1, front->right->val);
        } else {
            vec.insert(vec.end(), 1, 0);
        }
    }
    return vec;
}

vector<int> get_vec_tree_no_zero(TreeNode* root) {
    // 中序遍历
    stack<TreeNode*> s;
    vector<int> vec;
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
        if (cur->val) vec.push_back(cur->val);
        s.pop();
        if (cur->right) {
            s.push(cur->right);
            cur = cur->right;
        }
    }
    return vec;
}

void printTree(vector<int> tree_vec) {
    int height = 0, l = 1;
    while (l <= tree_vec.size()) {
        l *= 2;
        height++;
    }
    int layer = 1, interval = l;
    for (int i = 0; i < l - 1; i = i * 2 + 1) {
        layer++;

        for (int k = 0; k < interval / 2; k++) cout << ' ';
        for (int j = i; j < i * 2 + 1; j++) {
            if (tree_vec[j] && j < tree_vec.size())
                cout << tree_vec[j];
            else
                cout << ' ';
            for (int k = 0; k < interval - 1; k++) cout << ' ';
        }
        interval = interval / 2;
        cout << endl;
    }
    return;
}