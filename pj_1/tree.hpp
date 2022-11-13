#ifndef TREE_H_
#define TREE_H_

#include <cassert>
#include <iostream>
#include <string>

#include "item.hpp"

using namespace std;

struct TreeNode {
    // left: son, right:sibling
    TreeNode* sibling;
    TreeNode* son;
    TreeNode* parent;
    Item* item;
    int data;
    TreeNode() {
        item = nullptr;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
    }
    TreeNode(Item* n) {
        item = n;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
    }
};

class CartTree {
   protected:
    TreeNode* root_;
    void __AddItemRecur(TreeNode*, Item*);
    int __SubTreePriority(TreeNode*, Item*);
    void __ShowCartRecur(TreeNode*);
    void __ShowTreeRecur(TreeNode*);
    void __DelTree(TreeNode* node);
    bool __NoAncestor(int, TreeNode*);
    TreeNode* __get_prev_sibling(TreeNode*);
    int last_time;

   public:
    CartTree();
    ~CartTree() { __DelTree(root_); };
    void AddItem(Item*);
    // delete: ref with ptr
    void DelItem(Item*);
    // delete: ref with index
    void DelItem(int shop_id, int item_id);
    // delete: ref with name
    void DelItem(string name);
    void ShowCart() { __ShowCartRecur(root_); };
    void ShowTree() { __ShowTreeRecur(root_); };
};

CartTree::CartTree() {
    TreeNode* root_node = new TreeNode();
    root_ = root_node;
    last_time = 0;
    return;
}

void CartTree::__DelTree(TreeNode* node) {
    if (node != nullptr) {
        __DelTree(node->sibling);
        __DelTree(node->son);
        delete node;
    }
    return;
}

TreeNode* CartTree::__get_prev_sibling(TreeNode* node) {
    assert(node->parent != nullptr);
    TreeNode* cur = node->parent->son;
    while (cur != nullptr && cur->sibling != node) {
        cur = cur->sibling;
    }
    // if node is the first sibling, should return nullptr.
    return cur;
}

void CartTree::__ShowTreeRecur(TreeNode* cur) {
    if (cur == nullptr) return;

    // cout << cur << endl;
    cout << endl << endl << "Tree Node:" << endl;
    cout << "node add " << cur << endl;
    cout << "node data " << cur->data << endl;
    cout << "node item add " << cur->item << endl;
    if (cur->item != nullptr && cur->data == -1) {
        cout << "item:" << endl;
        cur->item->ShowItem();
    }
    __ShowTreeRecur(cur->son);
    __ShowTreeRecur(cur->sibling);
}
void CartTree::__ShowCartRecur(TreeNode* cur) {
    // 为啥debug的时候，这里没有加一行cout的话就不会报exception
    if (cur == nullptr) return;

    // cout << cur << endl;
    if (cur->son != nullptr) {
        __ShowCartRecur(cur->son);
    } else {
        cur->item->ShowItem();
    }
    __ShowCartRecur(cur->sibling);
}

void CartTree::AddItem(Item* item) {
    /*
     * The tree is arranged as described here.
     * FROM BOTTOM TO TOP:
     * Leaves: all items in the cart
     *   data: -1
     *
     * 1-th layer parents:
     *   item: ptr to the descendant with the most recent add_time
     *   data: an int determining sorting value (e.g., item->discount_1[0](where
     *   data == 0), (where data == MAX_DISCOUNT_1), item->discount_2[3] (where
     *   data == MAX_DISCOUNT_1 + 4), etc.).
     *
     *   The descendants of a node in this layer are sorted  according to least
     *   prior sorting condition, i.e. "跨店满减". From left to right, the nodes
     *   are stored in ascending order of item->add_time
     *
     * 2-th layer:
     *   item: same
     *   data: same
     *   The descendants of a node in this layer are sorted  according to second
     *   least prior sorting condition, i.e. another "跨店满减" (if there are
     *   multiple ones), or the next rule, "Same-Store". From left to right, the
     *   nodes are stored in ascending order of item->add_time
     *
     * 3-th layer...
     *
     * root: empty, item == nullptr
     */
    // Now, from top to bottom, traverse the tree. Look for subtrees to add the
    // new node. If a certain subtree is to be inserted, item of this node
    // points to the latest item.

    // find place to insert
    __AddItemRecur(root_, item);
    last_time += 1;
    return;
}

void CartTree::__AddItemRecur(TreeNode* root, Item* item) {
    // OK
    // 这个错了...可能高优先级的节点在低优先级的子树下？至少O(N)了这下
    TreeNode *cur, *cur_parent, *next_root = nullptr;
    int this_priority, best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    // 找到最高优先级的那个叶子节点，插入新节点
    // 或者在寻找过程中，已经不可能存在new node
    // 和某个叶子节点组成的，不包含在祖先里的新节点。
    while (root->son != nullptr) {
        cur = root->son;
        // 遍历root下的所有孩子，找到最高优先级的那个孩子
        while (cur != nullptr) {
            this_priority = __SubTreePriority(root, item);
            // 要求：找到的相同优先级是“新”的，也就是和root以及它的祖先都不同
            if (best_priority > this_priority &&
                __NoAncestor(this_priority, root)) {
                // this is the next root
                best_priority = this_priority;
                next_root = cur;
            }
            cur = cur->sibling;
        }
        // 如果一个相同discount都没有，加到root的最左边就行
        if (!next_root) {
            TreeNode* new_node = new TreeNode(item);
            new_node->sibling = root->son;
            new_node->parent = root;
            root->son = new_node;
            // 添加完成了
            return;
        }
        // 由于新加的节点是最新的，所以把next root移到siblings中的最左边
        // 如果nextroot不是最左孩子，维护sibling指针
        cur = __get_prev_sibling(next_root);
        if (cur != nullptr) {
            cur->sibling = next_root->sibling;
        }
        next_root->sibling = root->son->sibling;
        root->son = next_root;
        // 更新item
        next_root->item = item;

        root = next_root;
    }
    // 第一次添加
    if (root == root_) {
        TreeNode* new_node = new TreeNode(item);
        new_node->parent = root_;
        root_->son = new_node;
        return;
    }
    // 此时：root现在是某个叶子，且这个newnode+这个叶子一定能形成比它的parent更高的优先级
    // (如果这个叶子没有和newnode一样的新优先级，那上次while中应该就已经把newnode接上，成为叶子的sibling了)
    //      new_root
    //      /   \
    //    /       \
    // new_node   root
    cur_parent = root->parent;
    cur = __get_prev_sibling(root);
    if (cur != nullptr) {
        cur->sibling = root->sibling;
    } else {
        // root 是第一个孩子
        root->parent->son = root->sibling;
    }
    root->sibling = nullptr;
    TreeNode* new_root = new TreeNode(item);
    TreeNode* new_node = new TreeNode(item);
    new_root->son = new_node;
    new_root->data = best_priority;
    new_node->parent = new_root;
    new_node->sibling = root;
    root->parent = new_root;

    while (cur_parent->parent != nullptr && best_priority < cur_parent->data) {
        // bestprior不可能等于cur data，要不前面那个NoAncestor的条件一定过不了
        assert(best_priority != cur_parent->data);
        cur_parent = cur_parent->parent;
    }
    // best_p > cur->data, new_root 要加在cur的下面
    new_root->parent = cur_parent;
    new_root->sibling = cur_parent->son;
    cur_parent->son = new_root;
    // 加完了
    return;
}

int CartTree::__SubTreePriority(TreeNode* root, Item* item) {
    /*
        找到当前节点为根的子树中，和item相符合的最高优先级的discount
        遍历root下的所有叶子，返回最高的优先级
    */
    // exit
    if (root->son == nullptr) {
        // 子树就是这个叶子，所以只要判断一个节点
        // 脑袋不清不楚的老是写一些冗余代码 :(
        for (int j = 0; j < MAX_DISCOUNT_1; j++) {
            // all possible 折扣
            if (item->discount_1[j] && root->item->discount_1[j]) {
                return j;
            }
        }
        if (item->shop_id == root->item->shop_id) {
            // same shop
            return MAX_DISCOUNT_1;
        }
        for (int j = 0; j < MAX_DISCOUNT_2; j++) {
            // all possible 满减
            if (item->discount_2[j] && root->item->discount_2[j]) {
                return j + MAX_DISCOUNT_1 + 1;
            }
        }
        // 一个一样的discount/商店都没
        return MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    }
    // Not leaf, recursion
    TreeNode *best_son, *cur = root->son;
    int this_priority, best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    while (cur != nullptr) {
        this_priority = __SubTreePriority(cur, item);
        if (this_priority < best_priority) {
            best_priority = this_priority;
            best_son = cur;
        }
        cur = cur->sibling;
    }
    return best_priority;
}

bool CartTree::__NoAncestor(int data, TreeNode* node) {
    while (node != root_) {
        if (node->data == data) return false;
        node = node->parent;
    }
    return true;
}

#endif