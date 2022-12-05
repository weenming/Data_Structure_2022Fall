#include <cassert>
#include <iostream>
#include <vector>

#include "vector2tree.hpp"
using namespace std;

int height(TreeNode* cur) {
    if (!cur) return -1;
    return max(height(cur->left), height(cur->right)) + 1;
}

TreeNode* find_rot_pivot(TreeNode* cur) {
    if (!cur) return NULL;
    if (abs(height(cur->left) - height(cur->right)) > 1) {
        return cur;
    }
    TreeNode* rot_pivot = find_rot_pivot(cur->parent);
    if (rot_pivot && cur->parent->parent == rot_pivot)
        return cur;
    else
        return rot_pivot;
}

TreeNode* balance_tree(TreeNode* cur, TreeNode* root) {
    TreeNode* t = find_rot_pivot(cur);
    if (!t) return root;  // no need to rotate
    // 4 cases
    TreeNode *s = t->parent, *n = s->parent;
    if (t == s->left && s == s->parent->left) {
        // right single rot
        s->parent = n->parent;
        n->left = s->right;
        s->right = n;
        n->parent = s;
        if (n == root)
            return s;
        else {
            // s->parent is what n->parent was before balancing
            if (s->parent->left == n) {
                s->parent->left = s;
            } else {
                assert(s->parent->right == n);
                s->parent->right = s;
            }
            return root;
        }

    } else if (t == s->right && s == s->parent->right) {
        // left single rot
        s->parent = n->parent;
        n->right = s->left;
        s->left = n;
        n->parent = s;
        if (n == root)
            return s;
        else {
            if (s->parent->left == n) {
                s->parent->left = s;
            } else {
                assert(s->parent->right == n);
                s->parent->right = s;
            }
            return root;
        }
    } else if (t == s->right && s == n->left) {
        // l-r double
        // left, around s
        t->parent = s->parent;
        s->right = t->left;
        t->left = s;
        s->parent = t;
        // right, around t (s->t->n)
        t->parent = n->parent;
        n->left = t->right;
        t->right = n;
        n->parent = t;
        if (n == root)
            return t;
        else {
            if (t->parent->left == n) {
                t->parent->left = t;
            } else {
                assert(t->parent->right == n);
                t->parent->right = t;
            }
            return root;
        }
    } else if (t == s->left && s == n->right) {
        // r-l double
        // right, around s
        t->parent = s->parent;
        s->left = t->right;
        t->right = s;
        s->parent = t;
        // left, around t (s->t->n)
        t->parent = n->parent;
        n->right = t->left;
        t->left = n;
        n->parent = t;
        if (n == root)
            return t;
        else {
            if (t->parent->left == n) {
                t->parent->left = t;
            } else {
                assert(t->parent->right == n);
                t->parent->right = t;
            }
            return root;
        }
        return root;
    } else {
        assert(false);
    }
}

TreeNode* add_to_AVL_tree(TreeNode* cur, TreeNode* root, TreeNode* tree_root) {
    if (cur->val >= root->val && !root->right) {
        root->right = cur;
        cur->parent = root;
        return balance_tree(cur, tree_root);
    } else if (cur->val <= root->val && !root->left) {
        root->left = cur;
        cur->parent = root;
        return balance_tree(cur, tree_root);
    } else if (cur->val < root->val) {
        return add_to_AVL_tree(cur, root->left, tree_root);
    } else if (cur->val > root->val) {
        return add_to_AVL_tree(cur, root->right, tree_root);
    } else {
        cout << "should not run here" << endl;
        assert(false);
    }
}

TreeNode* add_to_tree_recur(TreeNode* cur, TreeNode* root) {
    if (!cur) return root;
    TreeNode *left = cur->left, *right = cur->right;
    cur->left = NULL;
    cur->right = NULL;
    if (cur != root && cur->val) {
        root = add_to_AVL_tree(cur, root, root);
        printTree(get_vec_tree(root));
        cout << endl;
    }
    root = add_to_tree_recur(left, root);
    root = add_to_tree_recur(right, root);
    return root;
}

TreeNode* mySolution(TreeNode* root) {
    // 最坏的时间复杂度 O(N^2), 因为求平衡因子要遍历，O(N)
    // 空间复杂度 O(log_2 N)，遍历时的栈高度
    root->parent = NULL;
    root = add_to_tree_recur(root, root);
    // TreeNode *left = root->left, *right = root->right;
    // root->left = NULL;
    // root->right = NULL;
    // add_to_tree_recur(left, root);
    // add_to_tree_recur(right, root);
}

TreeNode* CreateTreeRecur(vector<int> vec, int start, int end) {
    if (start == end) {
        return nullptr;
    } else if (start == end - 1) {
        TreeNode* root = new TreeNode(vec[start]);
        return root;
    }
    int mid = (start + end) / 2;
    TreeNode* root = new TreeNode(vec[mid]);
    TreeNode* lnode = CreateTreeRecur(vec, start, mid);
    TreeNode* rnode = CreateTreeRecur(vec, mid, end);
    root->left = lnode;
    root->right = rnode;
    return root;
}

TreeNode* myNewSol(TreeNode* root) {
    vector<int> vec = get_vec_tree_no_zero(root);
    for (int i = 0; i < vec.size(); i++) cout << vec[i] << ' ';
    cout << endl;
    return CreateTreeRecur(vec, 0, vec.size());
}

void test() {
    vector<int> a = {1,    NULL, 2,    NULL, NULL, NULL, 3, NULL,
                     NULL, NULL, NULL, NULL, NULL, NULL, 4};

    // 已经平衡的
    // vector<int> a = {4, 2, 6, 1, 3, 5, 7};

    // 右旋转
    // vector<int> a = {10, 6, 12, 5, 8, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 3};

    // 先左后右。这个打印树的偏移量有点问题，因为平衡二叉树并不意味着只有最后一层有空节点...
    // 似乎要把二叉树补成满的再打印。直接去看指针能发现是对的
    // vector<int> a = {7, 5, 8, 2, 6, 0, 0, 1, 3, 0, 9, 0, 0, 0, 0, 0, 0, 0,
    // 4};

    // 有子树的左旋转
    // vector<int> a = {5, 1, 6, 0, 2, 0, 7, 0, 0, 0, 3};

    TreeNode* root = createTree(a, 0);
    cout << "before" << endl;
    printTree(get_vec_tree(root));
    root = myNewSol(root);
    cout << "after" << endl;
    printTree(get_vec_tree(root));
}

int main() {
    test();
    return 0;
}