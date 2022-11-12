#include <iostream>
#include <string>

using namespace std;

template <typename T>
struct TreeNode {
    // left: son, right:sibling
    TreeNode<T>* sibling;
    TreeNode<T>* son;
    T* data;
    TreeNode() {
        data = nullptr;
        sibling = nullptr;
        son = nullptr;
    }
    TreeNode(T* n) {
        data = n;
        sibling = nullptr;
        son = nullptr;
    }
};