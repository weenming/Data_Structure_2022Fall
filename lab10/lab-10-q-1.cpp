#include <iostream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right)
        : val(x), left(left), right(right) {}
};

int recur_bf(TreeNode *cur) {
    if (!cur) return 0;
    int l = recur_bf(cur->left);
    int r = recur_bf(cur->right);
    if (l == -1 || r == -1) {
        // when either child is imbalanced
        return -1;
    } else if ((abs(l - r) > 1)) {
        return -1;
    } else {
        // return height of cur
        return max(l, r) + 1;
    }
}

bool Solution(TreeNode *root) {
    if (recur_bf(root) != -1)
        return true;
    else
        return false;
}

void test1() {
    bool ans = Solution(nullptr);
    if (ans)
        cout << "True!" << endl;
    else
        cout << "False!" << endl;
    return;
}

void test2() {
    TreeNode *n4 = new TreeNode(7);
    TreeNode *n3 = new TreeNode(15);
    TreeNode *n1 = new TreeNode(9);
    TreeNode *n2 = new TreeNode(20, n3, n4);
    TreeNode *root = new TreeNode(3, n1, n2);

    bool ans = Solution(root);
    if (ans)
        cout << "True!" << endl;
    else
        cout << "False!" << endl;
    return;
}

void test3() {
    TreeNode *n5 = new TreeNode(233);
    TreeNode *n4 = new TreeNode(7, nullptr, n5);
    TreeNode *n3 = new TreeNode(15);
    TreeNode *n1 = new TreeNode(9);
    TreeNode *n2 = new TreeNode(20, n3, n4);
    TreeNode *root = new TreeNode(3, n1, n2);

    bool ans = Solution(root);
    if (ans)
        cout << "True!" << endl;
    else
        cout << "False!" << endl;
    return;
}

void test4() {
    // imbalance in subtree
    /*
        root
        /  \
      n1    n2
     / \      \
   n10 n11     n4
                |
                n3
    */
    TreeNode *n10 = new TreeNode(10);
    TreeNode *n11 = new TreeNode(11);
    TreeNode *n3 = new TreeNode(15);
    TreeNode *n4 = new TreeNode(7, nullptr, n3);
    TreeNode *n1 = new TreeNode(9, n10, n11);
    TreeNode *n2 = new TreeNode(20, nullptr, n4);
    TreeNode *root = new TreeNode(3, n1, n2);

    bool ans = Solution(root);
    if (ans)
        cout << "True!" << endl;
    else
        cout << "False!" << endl;
    return;
}

int main() {
    cout << "test1:" << endl;
    test1();

    cout << "test2:" << endl;
    test2();

    cout << "test3:" << endl;
    test3();

    cout << "test3:" << endl;
    test4();

    return 0;
}