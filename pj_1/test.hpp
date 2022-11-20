#ifndef TEST_H_
#define TEST_H_

#include <cassert>
#include <iostream>
#include <string>

#include "item.hpp"
#include "tree.hpp"

using namespace std;

class TestTree : CartTree {
   public:
    int TestPriority(TreeNode*, Item*, TreeNode*);
    void TestAdd(bool, Item*);
    void TestDel(int, int);
    void ShowTree(TreeNode*);
    void ShowTreePretty(TreeNode*, string);
};

int TestTree::TestPriority(TreeNode* node, Item* item, TreeNode* root) {
    root_ = root;
    return __SubTreePriority(node, item, node);
}

void TestTree::TestAdd(bool show, Item* item) {
    AddItem(item);

    if (show) {
        cout << "showing tree" << endl << endl;
        ShowTreePretty(root_, "");
    }
    // cout << "showing cart" << endl << endl;
    // __ShowCartRecur(root);
    return;
}

void TestTree::TestDel(int shop_id, int item_id) {
    cout << "before del" << endl;
    ShowTreePretty(root_, "");
    DelItem(shop_id, item_id);
    cout << "after del" << endl;
    ShowTreePretty(root_, "");
    // cout << "showing cart" << endl << endl;
    // __ShowCartRecur(root);
    return;
}

void TestTree::ShowTree(TreeNode* cur) {
    if (cur == nullptr) return;

    // cout << cur << endl;
    cout << endl << endl << "Tree Node:" << endl;
    cout << "node data " << cur->data << endl;
    cout << "node add " << cur << endl;
    cout << "son node add" << cur->son << endl;
    cout << "next sibling node add" << cur->sibling << endl;
    // cout << "node item add " << cur->item << endl;
    if (cur->item != nullptr && cur->son == nullptr) {
        cout << "item:" << endl;
        cur->item->ShowItem();
    }
    ShowTree(cur->son);
    ShowTree(cur->sibling);
}

void TestTree::ShowTreePretty(TreeNode* cur, string indent) {
    if (cur == nullptr) {
        return;
    }
    if (cur->data != -1) {
        cout << indent << "|- Node: data = " << cur->data;
        if (cur->data < MAX_DISCOUNT_1) {
            cout << " (跨店折扣" << cur->data + 1 << "-"
                 << cur->item->discount_1[cur->data] << ")" << endl;
        } else if (cur->data == MAX_DISCOUNT_1) {
            cout << " (同商店" << cur->item->shop_id << ")" << endl;
        } else {
            cout << " (跨店满减" << cur->data - MAX_DISCOUNT_1 << "-"
                 << cur->item->discount_2[cur->data - MAX_DISCOUNT_1 - 1] << ")"
                 << endl;
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

// void TestTree::ConstructTestTree() {
//     Item* item1 = new Item(1, 1, 1, 1);

//     Item* item2 = new Item(2, 2, 1, 0);
//     Item* item3 = new Item(2, 3, 2, 1);
//     Item* item4 = new Item(2, 4, 2, 0);
//     Item* item5 = new Item(5, 5, 2, 0);
//     Item* item6 = new Item(1, 6, 3, 0);

//     Item* item7 = new Item(2, 7, 1, 2);
//     Item* item8 = new Item(1, 8, 1, 1);
//     Item* item9 = new Item(2, 9, 1, 2);
//     Item* item10 = new Item(1, 10, 2, 1);
//     Item* item11 = new Item(1, 11, 2, 1);
//     item11->discount_1[1] = 1;
//     Item* item12 = new Item(1, 12, 2, 1);
//     Item* item13 = new Item(1, 13, 2, 1);
//     Item* item14 = new Item(1, 14, 2, 1);
//     item13->discount_1[1] = 1;
//     Item* item1_shop3 = new Item(3, 1, 2, 0);
//     Item* item2_shop3 = new Item(3, 2, 1, 0);
//     Item* item3_shop3 = new Item(3, 3, 2, 0);
//     Item* item4_shop3 = new Item(3, 4, 1, 0);
//     Item* item14_no_discount = new Item(14, 14, 0, 0);
//     Item* item15_no_discount = new Item(15, 15, 0, 0);
//     Item* item16_no_discount = new Item(16, 16, 0, 0);
//     Item* item17_discount_2 = new Item(111, 17, 0, 1);
//     Item* item18_discount_2 = new Item(111, 18, 0, 0);
//     Item* item19_discount_2 = new Item(222, 19, 0, 1);

//     CartTree* tree = new CartTree();
//     TreeNode* node1 = new TreeNode(item1);
//     TreeNode* node2 = new TreeNode(item2);
//     TreeNode* node3 = new TreeNode(item3);
//     TreeNode* node4 = new TreeNode(item4);
//     TreeNode* node5 = new TreeNode(item5);
//     TreeNode* node6 = new TreeNode(item6);
//     TreeNode* node_mid1 = new TreeNode();
//     TreeNode* node_mid2 = new TreeNode();
//     TreeNode* node_mid3 = new TreeNode();

//     TreeNode* root = tree->get_root();
//     root->son = node_mid1;
//     node_mid1->sibling = node_mid2;
//     node_mid2->sibling = node6;

//     node_mid1->son = node1;
//     node1->sibling = node2;

//     node_mid2->son = node_mid3;
//     node_mid3->sibling = node5;
//     node_mid3->son = node3;
//     node3->sibling = node4;

//     node1->parent = node_mid1;
//     node2->parent = node_mid1;
//     node3->parent = node_mid3;
//     node4->parent = node_mid3;
//     node5->parent = node_mid2;
//     node6->parent = root;
//     node_mid1->parent = root;
//     node_mid2->parent = root;
//     node_mid3->parent = node_mid2;

//     node_mid1->data = 0;
//     node_mid2->data = 0;
//     node_mid3->data = 10;
//     node_mid1->item = item1;
//     node_mid2->item = item3;
//     node_mid3->item = item3;
// }
#endif