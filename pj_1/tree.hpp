#ifndef TREE_H_
#define TREE_H_

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "globals.hpp"
#include "item.hpp"

using namespace std;

struct TreeNode {
    // left: son, right:sibling
    TreeNode* sibling;
    TreeNode* son;
    TreeNode* parent;
    Item* item;
    int data;
    // only leaves have non-zero item number...
    int number;
    int add_time;
    TreeNode() {
        // for initializing mid nodes
        item = nullptr;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
        number = 0;
    }
    TreeNode(Item* n, int time) {
        // for initializing leaves
        item = n;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
        number = 1;
        add_time = time;
    }
};

class CartTree {
   protected:
    TreeNode* root_;
    int __SubTreePriority(TreeNode*, Item*, TreeNode*);
    double __ShowCartRecur(TreeNode*);
    void __DelTree(TreeNode*);
    TreeNode* __AddItem(Item* item, bool update);
    TreeNode* __AddMidNode(TreeNode*, Item*);
    bool __NoAncestor(int, TreeNode*);
    TreeNode* __GetPrevSibling(TreeNode*);
    TreeNode* __FindLeaf(int shop_id, int item_id, TreeNode* root);
    TreeNode* __DelBadNodes(TreeNode*);
    TreeNode* __DelEmptyNodes(TreeNode*);
    void __UpdateMidNodesDel(int, TreeNode*);
    void __UpdateMidNodesAdd(TreeNode*);
    bool __SamePriority(TreeNode*, Item*);
    int last_time_;
    int item_count_;

   public:
    CartTree();
    ~CartTree() { __DelTree(root_); };
    void AddItem(Item*);
    TreeNode* get_root();
    // delete: ref with ptr
    void DelItem(Item*);
    // delete: ref with index
    void DelItem(int shop_id, int item_id);
    // delete: ref with name
    void DelItem(string name);
    void ShowCart() {
        cout << "Showing Shopping cart:\n" << endl;
        cout << "Total price: " << fixed << setprecision(2)
             << __ShowCartRecur(root_) << endl
             << endl;
    };
    void ShowTreePretty(TreeNode*, string);
};

void CartTree::ShowTreePretty(TreeNode* cur, string indent) {
    if (cur == nullptr) {
        return;
    }
    if (cur->data != -1) {
        cout << indent << "|- Node: data = " << cur->data;
        if (cur->data < MAX_DISCOUNT_1) {
            cout << " (????????????" << cur->data + 1 << "-"
                 << char('a' - 1 + cur->item->discount_1[cur->data]) << ")"
                 << endl;
        } else if (cur->data == MAX_DISCOUNT_1) {
            cout << " (?????????" << cur->item->shop_id << ")" << endl;
        } else {
            cout << " (????????????" << cur->data - MAX_DISCOUNT_1 << "-"
                 << char('a' - 1 +
                         cur->item->discount_2[cur->data - MAX_DISCOUNT_1 - 1])
                 << ")" << endl;
        }
    } else if (cur->item != nullptr) {
        // leaf
        cout << indent << "|- Leaf-"
             << "-item number: " << cur->number
             << " add time: " << cur->add_time << endl;
        cur->item->ShowItem(indent + "*  ");

    } else {
        // cur->item == nullptr: should be root
        cout << indent << "|- Root " << endl;
    }
    ShowTreePretty(cur->son, indent + "*    ");
    ShowTreePretty(cur->sibling, indent);
}

TreeNode* CartTree::get_root() { return root_; }

CartTree::CartTree() {
    TreeNode* root_node = new TreeNode();
    root_ = root_node;
    last_time_ = 0;
    item_count_ = 0;
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

double CartTree::__ShowCartRecur(TreeNode* cur) {
    if (cur == nullptr) return 0;

    // cout << cur << endl;
    if (cur->son != nullptr) {
        return __ShowCartRecur(cur->son) + __ShowCartRecur(cur->sibling);
    } else if (cur != root_) {
        cur->item->ShowItemForCart();
        cout << "      - " << cur->number << " pcs in your shopping cart"
             << endl;
        cout << "      - "
             << "   add time: " << cur->add_time << endl;
        cout << "   price: " << fixed << setprecision(2) << cur->item->price
             << " * " << cur->number << endl
             << endl;
        return cur->item->price * cur->number + __ShowCartRecur(cur->sibling);
    } else {
        return 0;
    }
}

void CartTree::AddItem(Item* item) {
    cout << "Adding item: id " << item->item_id << " ..." << endl;
    if (item_count_ < MAX_ITEM_NUMBER) {
        // ?????????????????????????????????????????????
        last_time_ += 1;
        item_count_ += 1;
        __AddItem(item, true);
        cout << "Add item success" << endl << endl;
        return;
    } else {
        cout << "Fail: shopping cart is full!" << endl << endl;
        return;
    }
}
TreeNode* CartTree::__AddItem(Item* item, bool update) {
    /*
     * The tree is arranged as described here.
     * FROM BOTTOM TO TOP:
     * Leaves: all items in the cart
     *   data: -1
     *
     * 1-th layer parents:
     *   item: ptr to the descendant's item with the most recent add_time
     *   data: an int determining sorting value (e.g., item->discount_1[0](where
     *   data == 0), (where data == MAX_DISCOUNT_1), item->discount_2[3] (where
     *   data == MAX_DISCOUNT_1 + 4), etc.).
     *
     *   The descendants of a node in this layer are sorted  according to least
     *   prior sorting condition, i.e. "????????????". From left to right, the nodes
     *   are stored in ascending order of item->add_time
     *
     * 2-th layer:
     *   item: same
     *   data: same
     *   The descendants of a node in this layer are sorted  according to second
     *   least prior sorting condition, i.e. another "????????????" (if there are
     *   multiple ones), or the next rule, "Same-Store". From left to right, the
     *   nodes are stored in ascending order of item->add_time
     *
     * 3-th layer...
     *
     * root: empty, item == nullptr


    ??????????????????????????????????????????????????????????????????A??????????????????(B1, B2,
    B3...)????????????????????????
    a.
    ??????Bn????????????????????????Bn???item????????????A????????????????????????????????????????????????????????????

    b.
    ??????Bn?????????????????????
        b1. ??????Bn?????????????????????item??????
            ??????Bn??????????????????????????????????????????????????????????????????
        b2. ??????????????????????????????????????????(MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1)

    ????????????????????????
        some_node --- with same discount_1[1] as item to add
        |
        |
        cur --- with same discount_2[0] as item to add
       /    \      \
    item2    item3   \
    (shop2)  (shop3)   some_other_node (same discount_1[2]) as item to add
                        \
                        item4 (shop4)
    (1) when item to add: item->shop_id != 2 and item->shop_id != 3,
    item should be added as a direct leaf of cur.

    (2) cur????????????????????????????????????????????????item?????????????????????????????????????????????

    (3) cur????????????????????????????????????????????????item????????????????????????????????????????????????
        ?????????????????????????????????????????????item?????????????????????????????????????????????????????????
        ?????????

    (4)
    ????????????cur???????????????????????????????????????item???????????????????????????????????????????????????
    ????????????????????????????????????????????????????????????

    when item to add: item->shop_id == 2, a new node should be
    created
            some_node --- with same discount_1[0] as item to add
                |       \
                |       ...
                cur --- with same discount_2[0] as item to add
                /    \
            new_node    \
            /    \         \
        item2   new_item    item3
        (shop2)  (shop2)      (shop3)
        In fact the new node may have higher priority, and thus should
        be added to the preceding positions in the tree. This does not
        matter because a backward search will be executed afterwards.

    (5) when item to add has the same shop_id and item_id, they are
        considered an identical item. Visit the next node and it can be
        eventually handled.
    ???????????????leaf???item??????????????????????????????????????????????????????????????????????????????
    ???????????????????????????????????????

    ??????
    ?????????????????????????????????newnode+????????????????????????????????????parent???????????????????????????
    ??????????????????????????????item?????????????????????????????????????????????????????????????????????????????????mid
    nodes

    (???????????????????????????newnode?????????????????????????????????while?????????????????????newnode????????????????????????sibling???)

    ????????????????????????????????????
        mid_node_1 --- ???item????????????????????????????????????????????????
            |
            :
            :
            |
        mid_node_n --- ???n???????????????
    -------???????????????????????????------
          /   \
         /     \
       root    node_to_add (construct with item)


    */

    // Now, from top to bottom, traverse the tree. Look for subtrees to add the
    // new node. If a certain subtree is to be inserted, item of this node
    // points to the latest item.

    // find place to insert
    // ???????????????
    if (root_->son == nullptr) {
        TreeNode* new_node = new TreeNode(item, last_time_);
        new_node->parent = root_;
        root_->son = new_node;
        return new_node;
    }
    // ?????????????????????
    TreeNode* root = root_;
    // ??????????????????????????????????????????????????????...??????O(N)?????????
    TreeNode *cur, *cur_parent, *next_root;
    int this_priority, best_priority;
    // ????????????????????????????????????????????????????????????
    // ????????????????????????????????????????????????new node
    // ?????????????????????????????????????????????????????????????????????
    while (root->son != nullptr) {
        best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
        next_root = nullptr;
        cur = root->son;
        // ??????root?????????????????????????????????????????????????????????
        while (cur != nullptr) {
            /* SubTreePriority ????????????cur????????????????????????item????????????????????????
               cur???????????????????????????????????????????????????????????????cur???cur??????cur??????
               ?????????????????????
             */
            this_priority = __SubTreePriority(cur, item, cur);
            if (best_priority > this_priority) {
                // this is the next root
                best_priority = this_priority;
                next_root = cur;
            }
            cur = cur->sibling;
        }
        // ????????????????????????????????????discount??????????????????root??????????????????
        if (!next_root) {
            // ?????????????????????????????????????????????????????????????????????????????????????????????
            // subtreepriority?????????????????????...
            cur = root->son;
            while (cur) {
                if (cur->data == -1 && cur->item->shop_id == item->shop_id &&
                    cur->item->item_id == item->item_id) {
                    root = cur;
                    break;
                }
                cur = cur->sibling;
            }
            if (cur == root) break;

            TreeNode* new_node = new TreeNode(item, last_time_);
            new_node->sibling = root->son;
            new_node->parent = root;
            root->son = new_node;
            // ???????????????
            return new_node;
        }
        // ????????????????????????????????????
        else if (next_root->son == nullptr && update) {
            // ????????????????????????
            cur = __GetPrevSibling(next_root);
            if (cur != nullptr) {
                cur->sibling = next_root->sibling;
                next_root->sibling = root->son;
                root->son = next_root;
            }
            // ShowTreePretty(root_, "");
            // ??????????????????(??????)???
            root = next_root;
            break;
        }
        // ??????????????????????????????????????????
        else if (__SamePriority(next_root, item) && update) {
            // ?????????????????????????????????????????????
            // ?????????????????????????????????????????????next root??????siblings???????????????
            // ??????nextroot???????????????????????????sibling??????
            cur = __GetPrevSibling(next_root);
            if (cur != nullptr) {
                cur->sibling = next_root->sibling;
                next_root->sibling = root->son;
                root->son = next_root;
            }
            // ??????item
            next_root->item = item;
            next_root->add_time = last_time_;
            // ?????????????????????
            root = next_root;
        }
        // ????????????????????????????????????????????????????????????????????????
        else {
            // ??????????????????????????????????????????????????????????????????????????????????????????
            // ???????????????????????????????????????????????????????????????????????????????????????????????????
            // ???????????????????????????
            root = next_root;
        }
    }

    if (root->item->shop_id == item->shop_id &&
        root->item->item_id == item->item_id) {  // ???????????????
        root->number += 1;
        root->add_time = last_time_;
    } else {
        root = __AddMidNode(root, item);
    }
    return root;
}

TreeNode* CartTree::__AddMidNode(TreeNode* root, Item* item) {
    assert(root != nullptr && root->parent != nullptr);
    // ????????????????????????????????? ?????????????????????????????????????????????????????????
    // ????????????????????????????????????????????????????????????SubTreePriority ??????
    int best_priority, cur_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    TreeNode *first_mid_node = root->parent, *last_mid_node;
    assert(first_mid_node->son == root);
    /*
            (original tree)
                  |
             first_mid_node
                  |  \
                  :   \
                  :   siblings of root in the original tree
                  :
                  |
            last_mid_node
               /    \
           root   (new_node, not yet added)
           ?????????????????????21??????????????????
    */
    cur_priority = __SubTreePriority(root, item, root);
    assert(cur_priority != MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1);
    best_priority = cur_priority;
    while (__SubTreePriority(root, item, root) !=
           MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1) {
        cur_priority = __SubTreePriority(root, item, root);
        last_mid_node = new TreeNode();
        last_mid_node->item = item;
        last_mid_node->add_time = last_time_;
        last_mid_node->data = cur_priority;
        last_mid_node->parent = root->parent;
        last_mid_node->son = root;
        root->parent->son = last_mid_node;
        root->parent = last_mid_node;
    }
    // ??????????????????????????? ?????????root ?????????root->parent??????????????????
    // ????????????1????????????first_mid_node ???????????????last_mid_node
    first_mid_node = first_mid_node->son;
    assert(first_mid_node->data == best_priority);
    // ??????item?????????leaf
    TreeNode *original_parent = first_mid_node->parent,
             *original_sibling = root->sibling;
    /*
        cur_parent
        /         \
  mid_nodes         ...
       /            \
      root -> ... -> siblings
    */
    // ?????????root???sibling??????????????????sibling,??????parent???????????????
    // root ??????????????????
    assert(original_parent->son == first_mid_node);

    original_parent->son = root->sibling;
    root->sibling = nullptr;
    // ???item?????????leaf??????????????????son?????????
    TreeNode* new_node = new TreeNode(item, last_time_);
    last_mid_node->son = new_node;
    new_node->sibling = root;
    new_node->parent = last_mid_node;
    TreeNode *cur_parent = original_parent, *cur_insert;
    // ?????????????????????priority?????????????????????????????????????????????
    while (cur_parent->parent != nullptr && best_priority < cur_parent->data) {
        // bestprior???????????????cur
        // data?????????????????????NoAncestor????????????????????????
        assert(best_priority != cur_parent->data);
        /* ????????????????????????????????????original_parent ???????????????????????????
         * ??????????????????????????????????????????????????????(??????
         * ????????????mid')????????????????????????????????????????????????
         * ??????????????????root?????????????????????mid'?????????
         */
        //   ?????????????????????????????????????????????????????????????????????????????????????????????
        //   ?????????????????????????????????item???????????????????????????
        if (!__SamePriority(cur_parent, item)) {
            cur_parent = cur_parent->parent;
            continue;
        }
        //   ??????????????????????????????????????????????????????????????????????????????
        cur_insert = first_mid_node;
        while (cur_insert->son && cur_parent->data > cur_insert->data) {
            // ??????????????????????????????data
            cur_insert = cur_insert->son;
        }
        assert(cur_parent->data != cur_insert->data);
        /**
         * cur_insert -> node1
         *                |
         *               node2 / leaf
         * ?????????????????????node1??????
         */
        TreeNode* new_node = new TreeNode();
        new_node->item = item;
        new_node->add_time = last_time_;
        new_node->data = cur_parent->data;
        new_node->son = cur_insert;
        new_node->parent = cur_insert->parent;
        cur_insert->parent = new_node;
        // ???????????????????????????????????????
        new_node->parent->son = new_node;
        // ShowTreePretty(original_parent, "");
        cur_parent = cur_parent->parent;
    }
    // best_p > cur->data, first_mid_node ?????????cur?????????
    first_mid_node->parent = cur_parent;
    first_mid_node->sibling = cur_parent->son;
    cur_parent->son = first_mid_node;
    // ?????????
    // ??????????????????(????????????????????????????????????????????????)
    TreeNode* remain_node = __DelBadNodes(original_sibling);
    // ????????????efficient???????????????????????????
    if (remain_node) {
        // ??????????????????

        TreeNode* prev_node = __GetPrevSibling(remain_node);
        if (prev_node) {
            prev_node->sibling = remain_node->sibling;
        } else {
            remain_node->parent->son = remain_node->sibling;
        }
        // ??????????????????????????????
        int time_now = last_time_;
        last_time_ = remain_node->add_time;
        TreeNode* tmp_node = __AddItem(remain_node->item, false);
        last_time_ = time_now;
        remain_node->~TreeNode();
        remain_node = tmp_node;
        __UpdateMidNodesAdd(remain_node);

        return tmp_node;
    } else {
        return new_node;
    }
}

int CartTree::__SubTreePriority(TreeNode* root, Item* item, TreeNode* node0) {
    assert(root != nullptr);
    /*
        ??????????????????????????????????????????item???????????????????????????(node0)??????????????????????????????????????????????????????discount
        ??????root?????????????????????????????????????????????.
    */
    // exit
    if (root->son == nullptr) {
        // ?????????????????????????????????????????????????????????
        for (int j = 0; j < MAX_DISCOUNT_1; j++) {
            // ????????????????????????????????????
            if (item->discount_1[j] == root->item->discount_1[j] &&
                item->discount_1[j]) {
                if (__NoAncestor(j, node0)) {
                    return j;
                }
            }
        }
        if (item->shop_id == root->item->shop_id) {
            // same shop
            if (__NoAncestor(MAX_DISCOUNT_1, node0)) return MAX_DISCOUNT_1;
        }
        for (int j = 0; j < MAX_DISCOUNT_2; j++) {
            // all possible ??????
            if (item->discount_2[j] == root->item->discount_2[j] &&
                item->discount_2[j]) {
                if (__NoAncestor(j + MAX_DISCOUNT_1 + 1, node0))
                    return j + MAX_DISCOUNT_1 + 1;
            }
        }
        // ???????????????????????????????????????discount/????????????
        return MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    }
    // Not leaf, recursion

    int this_priority, best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    int tmp_priority = root->data;
    // ??????item???cur->data????????????cur->item ?????????????????????????????????
    // ??????root->data == -1?????????root_ (?????????????????????????????????)

    TreeNode *best_son, *cur = root->son;
    while (cur != nullptr) {
        this_priority = __SubTreePriority(cur, item, node0);
        if (this_priority < best_priority) {
            best_priority = this_priority;
            best_son = cur;
        }
        cur = cur->sibling;
    }
    // ????????????????????????(root)???????????????
    // ???????????????????????????(root)????????????node???????????????????????????????????????

    if (__SamePriority(node0, item)) {
        return best_priority;
    } else if (best_priority < node0->data) {
        return best_priority;
    } else {
        return MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    }
}

bool CartTree::__SamePriority(TreeNode* node, Item* item) {
    int tmp_priority = node->data;
    if (tmp_priority < MAX_DISCOUNT_1 && tmp_priority >= 0 &&
        node->item->discount_1[tmp_priority] == item->discount_1[tmp_priority])
        return true;
    else if (tmp_priority == MAX_DISCOUNT_1 &&
             node->item->shop_id == item->shop_id)
        return true;
    else if (tmp_priority > MAX_DISCOUNT_1 &&
             (node->item->discount_2[tmp_priority - MAX_DISCOUNT_1 - 1] ==
              item->discount_2[tmp_priority - MAX_DISCOUNT_1 - 1]))
        return true;

    return false;
}

bool CartTree::__NoAncestor(int data, TreeNode* node) {
    assert(node->parent != nullptr);
    // ??????????????????????????????????????????
    node = node->parent;
    while (node != root_) {
        if (node->data == data) return false;
        assert(node->parent != nullptr);
        node = node->parent;
    }
    return true;
}

TreeNode* CartTree::__DelBadNodes(TreeNode* leaf) {
    TreeNode *cur = leaf, *last_cur = leaf;
    if (leaf && leaf->parent && !leaf->son)
        // ??????leaf???nullptr, ????????????, ??????, ??????
        cur = leaf->parent;
    else
        return nullptr;
    // ?????????????????????leaf?????????????????????leaf?????????item???????????????????????????
    while (cur != root_) {
        if (!cur->son->sibling) {
            // ???????????????
            cur->item = leaf->item;
            cur->data = -1;
            cur->add_time = leaf->add_time;
            cur->number = leaf->number;
            cur->son->~TreeNode();
            cur->son = nullptr;
            last_cur = cur;
        } else
            break;
        cur = cur->parent;
    }
    return last_cur;
}

TreeNode* CartTree::__GetPrevSibling(TreeNode* node) {
    assert(node && node->parent);
    TreeNode* cur = node->parent->son;
    while (cur != nullptr && cur->sibling != node) {
        cur = cur->sibling;
    }
    // if node is the first sibling, should return nullptr.
    return cur;
}

void CartTree::DelItem(int shop_id, int item_id) {
    /**
     * ????????????
     * ???????????????shop_id and item_id
     * ??????????????????????????????
     *
     * (1) ????????????????????????
     *
     * (2) ????????????????????????1???, number-1 ????????????????????????add_item
     *          ??????????????????????????????...
     *          ???????????????????????????????????????????????????????????????+1????????????????????????????????????????????????
     *          ??????????????????????????????????????????...
     *
     * (3)   ????????????????????????1???????????????????????????
     *
     *    ??????leaf?????????sibling
     *    a. ?????????????????????root_?????????????????????????????????, ????????????????????????
     *    b. ????????????leaf???1???sibling???????????????sibling??????__DelBadNodes
     *       ??????????????????tmp_node
     *    c. ?????????2?????????sibling?????????????????????????????????
     *
     *    ??????????????????????????????????????????add_time:??????????????????????????????add_time,?????????
     *    ???????????????????????????sibling???add_time
     *    ????????????????????????sibling
     *
     *    ?????????????????????????????????????????????????????????????????????????????????(?????????sibling??????????????????
     *    ?????????????????????????????????????????????add_time??????????????????????????????)
     *
     *
     */
    item_count_ -= 1;
    TreeNode* leaf_to_del = __FindLeaf(shop_id, item_id, root_);
    if (!leaf_to_del) {
        cout << "can not find the item to remove!" << endl;
        return;
    } else {
        cout << "deleting item: id " << item_id << endl;
    }
    // cout << leaf_to_del->item->item_id << endl;
    // ??????????????????1?????????
    if (leaf_to_del->number > 1) {
        leaf_to_del->number -= 1;
        cout << "item " << item_id << " in shop " << shop_id << " deleted, "
             << leaf_to_del->number << "pcs of the same item remaining" << endl;
        return;
    }
    // ??????1?????????
    TreeNode *prev_node, *remain_node = nullptr, *tmp_node;
    // ??????????????????sibling
    // ????????????leaf????????????sibling?????????????????????????????????leaf
    // ????????????leaf???root_?????????
    if (!leaf_to_del->parent->son->sibling) {
        // ???????????????
        assert(leaf_to_del->parent == root_);
        leaf_to_del->~TreeNode();
        root_->son = nullptr;
        cout << "item " << item_id << " in shop " << shop_id
             << " deleted (last item in cart)" << endl;
        return;
    }

    assert(leaf_to_del->parent->son->sibling);
    // ????????????????????????????????????sort??????but whatever
    prev_node = __GetPrevSibling(leaf_to_del);
    if (prev_node) {
        // remain_node???leaf_to_del????????????????????????????????????
        remain_node = prev_node->parent->son;
        prev_node->sibling = leaf_to_del->sibling;
    } else {
        remain_node = leaf_to_del->sibling;
        leaf_to_del->parent->son = leaf_to_del->sibling;
    }
    // ?????????2???leaf?????????1?????????1??????????????????
    if (!leaf_to_del->parent->son->sibling && !leaf_to_del->parent->son->son) {
        remain_node = __DelBadNodes(remain_node);
        // ????????????efficient???????????????????????????
        // ??????????????????????????????item
        prev_node = __GetPrevSibling(remain_node);
        if (prev_node) {
            prev_node->sibling = remain_node->sibling;
        } else {
            remain_node->parent->son = remain_node->sibling;
        }
        // ??????????????????????????????
        int time_now = last_time_;
        last_time_ = remain_node->add_time;
        tmp_node = __AddItem(remain_node->item, false);
        last_time_ = time_now;
        remain_node->~TreeNode();
        remain_node = tmp_node;
    }
    // ???????????????????????????
    int deleted_add_time = leaf_to_del->add_time;
    leaf_to_del->~TreeNode();

    // ??????remain_node??????????????????add_time
    // ???remain_node????????????????????????????????????
    __UpdateMidNodesDel(deleted_add_time, remain_node->parent->son);
    cout << "item " << item_id << " in shop " << shop_id << " deleted, "
         << endl;
    return;
}

TreeNode* CartTree::__FindLeaf(int shop_id, int item_id, TreeNode* root) {
    if (!root) return nullptr;
    if (!root->son) {
        if (root->item->shop_id == shop_id && root->item->item_id == item_id)
            return root;
        else
            return __FindLeaf(shop_id, item_id, root->sibling);
    } else {
        TreeNode* leaf_in_son = __FindLeaf(shop_id, item_id, root->son);
        if (leaf_in_son) return leaf_in_son;
        TreeNode* leaf_in_sibling = __FindLeaf(shop_id, item_id, root->sibling);
        if (leaf_in_sibling) return leaf_in_sibling;
    }
    return nullptr;
}

void CartTree::__UpdateMidNodesDel(int deleted_add_time,
                                   TreeNode* remain_node) {
    int remain_add_time = remain_node->add_time;
    TreeNode *cur = remain_node, *prev_sibling;
    while (cur && cur->parent) {
        if (cur->add_time == deleted_add_time ||
            cur->add_time < remain_add_time) {
            cur->add_time = remain_add_time;
        }
        // ????????????????????????addtime??????????????????????????????
        while (cur->sibling && cur->add_time < cur->sibling->add_time) {
            prev_sibling = __GetPrevSibling(cur);
            if (prev_sibling) {
                prev_sibling->sibling = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                prev_sibling->sibling->sibling = cur;
            } else {
                cur->parent->son = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                cur->parent->son->sibling = cur;
            }
        }
        cur = cur->parent;
    }
    return;
}

void CartTree::__UpdateMidNodesAdd(TreeNode* remain_node) {
    int remain_add_time = remain_node->add_time;
    TreeNode *cur = remain_node, *prev_sibling;
    while (cur && cur->parent) {
        if (cur->add_time < remain_add_time) {
            cur->add_time = remain_add_time;
        }
        // ????????????????????????addtime??????????????????????????????
        while (cur->sibling && cur->add_time < cur->sibling->add_time) {
            prev_sibling = __GetPrevSibling(cur);
            if (prev_sibling) {
                prev_sibling->sibling = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                prev_sibling->sibling->sibling = cur;
            } else {
                cur->parent->son = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                cur->parent->son->sibling = cur;
            }
        }
        cur = cur->parent;
    }
    return;
}

#endif