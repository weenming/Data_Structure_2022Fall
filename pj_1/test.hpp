#ifndef TEST_H_
#define TEST_H_

#include <cassert>
#include <iostream>
#include <string>

#include "item.hpp"
#include "tree.hpp"

using namespace std;

class TestTree : public CartTree {
   public:
    int TestPriority(TreeNode*, Item*, TreeNode*);
    void TestAdd(bool, Item*);
    void TestDel(int, int);
    void ShowTree(TreeNode*);
    void ShowTreePretty(TreeNode*, string);
    void ConstructTestTree();
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
                 << char('a' - 1 + cur->item->discount_1[cur->data]) << ")"
                 << endl;
        } else if (cur->data == MAX_DISCOUNT_1) {
            cout << " (同商店" << cur->item->shop_id << ")" << endl;
        } else {
            cout << " (跨店满减" << cur->data - MAX_DISCOUNT_1 << "-"
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

void TestTree::ConstructTestTree() {
    Item* item1 = new Item(1, 1, 1, 1);

    Item* item2 = new Item(2, 2, 1, 0);
    Item* item3 = new Item(2, 3, 2, 1);
    Item* item4 = new Item(2, 4, 2, 0);
    Item* item5 = new Item(5, 5, 2, 0);
    Item* item6 = new Item(1, 6, 3, 0);

    Item* item7 = new Item(2, 7, 1, 2);
    Item* item8 = new Item(1, 8, 1, 1);
    Item* item9 = new Item(2, 9, 1, 2);
    Item* item10 = new Item(1, 10, 2, 1);
    Item* item11 = new Item(1, 11, 2, 1);
    item11->discount_1[1] = 1;
    Item* item12 = new Item(1, 12, 2, 1);
    Item* item13 = new Item(1, 13, 2, 1);
    Item* item14 = new Item(1, 14, 2, 1);
    item13->discount_1[1] = 1;
    Item* item1_shop3 = new Item(3, 1, 2, 0);
    Item* item2_shop3 = new Item(3, 2, 1, 0);
    Item* item3_shop3 = new Item(3, 3, 2, 0);
    Item* item4_shop3 = new Item(3, 4, 1, 0);
    Item* item14_no_discount = new Item(14, 14, 0, 0);
    Item* item15_no_discount = new Item(15, 15, 0, 0);
    Item* item16_no_discount = new Item(16, 16, 0, 0);
    Item* item17_discount_2 = new Item(111, 17, 0, 1);
    Item* item18_discount_2 = new Item(111, 18, 0, 0);
    Item* item19_discount_2 = new Item(222, 19, 0, 1);

    CartTree* tree = new CartTree();
    TreeNode* node1 = new TreeNode(item1, 1);
    TreeNode* node2 = new TreeNode(item2, 2);
    TreeNode* node3 = new TreeNode(item3, 3);
    TreeNode* node4 = new TreeNode(item4, 4);
    TreeNode* node5 = new TreeNode(item5, 5);
    TreeNode* node6 = new TreeNode(item6, 6);
    TreeNode* node_mid1 = new TreeNode();
    TreeNode* node_mid2 = new TreeNode();
    TreeNode* node_mid3 = new TreeNode();

    TreeNode* root = tree->get_root();
    root->son = node_mid1;
    node_mid1->sibling = node_mid2;
    node_mid2->sibling = node6;

    node_mid1->son = node1;
    node1->sibling = node2;

    node_mid2->son = node_mid3;
    node_mid3->sibling = node5;
    node_mid3->son = node3;
    node3->sibling = node4;

    node1->parent = node_mid1;
    node2->parent = node_mid1;
    node3->parent = node_mid3;
    node4->parent = node_mid3;
    node5->parent = node_mid2;
    node6->parent = root;
    node_mid1->parent = root;
    node_mid2->parent = root;
    node_mid3->parent = node_mid2;

    node_mid1->data = 0;
    node_mid2->data = 0;
    node_mid3->data = 10;
    node_mid1->item = item1;
    node_mid2->item = item3;
    node_mid3->item = item3;
}

void TestAdd() {
    Item item1(1, 1, 1, 0);
    Item item2(2, 2, 2, 0);
    Item item3(3, 3, 3, 0);
    CartTree cart1;
    cart1.AddItem(&item1);

    cart1.AddItem(&item2);

    cart1.AddItem(&item3);
    cart1.ShowCart();
}

void TestAddAndDel() {
    // shop_id, item_id, discount_1[0], discount_2[0]
    Item* item1 = new Item(1, 1, 1, 1);
    Item* item2 = new Item(2, 2, 1, 0);
    Item* item3 = new Item(2, 3, 2, 1);
    Item* item4 = new Item(2, 4, 2, 0);
    Item* item5 = new Item(5, 5, 2, 0);
    Item* item6 = new Item(1, 6, 3, 0);

    Item* item7 = new Item(2, 7, 1, 2);
    Item* item8 = new Item(1, 8, 1, 1);
    Item* item9 = new Item(2, 9, 1, 2);
    Item* item10 = new Item(1, 10, 2, 1);
    Item* item11 = new Item(1, 11, 2, 1);
    item11->discount_1[1] = 1;
    Item* item12 = new Item(1, 12, 2, 1);
    Item* item13 = new Item(1, 13, 2, 1);
    Item* item14 = new Item(1, 14, 2, 1);
    item13->discount_1[1] = 1;
    Item* item1_shop3 = new Item(3, 1, 2, 0);
    Item* item2_shop3 = new Item(3, 2, 1, 0);
    Item* item3_shop3 = new Item(3, 3, 2, 0);
    Item* item4_shop3 = new Item(3, 4, 1, 0);
    item4_shop3->discount_1[1] = 1;
    Item* item14_no_discount = new Item(14, 14, 0, 0);
    Item* item15_no_discount = new Item(15, 15, 0, 0);
    Item* item16_no_discount = new Item(16, 16, 0, 0);
    Item* item17_discount_2 = new Item(111, 17, 0, 1);
    Item* item18_discount_2 = new Item(111, 18, 0, 0);
    Item* item19_discount_2 = new Item(222, 19, 0, 1);
    Item* item20_no_discount = new Item(1, 20, 0, 0);
    Item* item21_no_discount = new Item(1, 21, 0, 0);
    Item* item22_no_discount = new Item(2, 22, 0, 0);
    Item* item23_no_discount = new Item(1, 23, 0, 0);
    Item* item24_discount_2 = new Item(333, 24, 0, 1);
    Item* item25_no_discount = new Item(100, 25, 0, 0);

    TestTree* test = new TestTree();
    /**
     * 随便加加
     */
    test = new TestTree();
    test->TestAdd(false, item1);
    test->TestAdd(false, item2);
    test->TestAdd(false, item3);
    test->TestAdd(false, item4);
    test->TestAdd(false, item5);
    test->TestAdd(false, item6);
    test->TestAdd(false, item7);

    test->TestAdd(false, item12);
    test->TestAdd(false, item12);
    test->TestAdd(false, item7);
    test->TestAdd(false, item7);
    test->TestAdd(false, item7);
    test->TestAdd(false, item8);
    test->TestAdd(false, item9);
    test->TestAdd(false, item10);
    test->TestAdd(false, item13);
    test->TestAdd(false, item4_shop3);
    test->TestAdd(false, item12);
    test->TestAdd(false, item11);
    test->TestAdd(true, item14);

    test->~TestTree();
    cout << "end of test 1" << endl << endl;
    /* 加几个discount2 1-2形成一个中间节点(各个discount1
    1-1不同)，然后再加一个
     * discount1 1-1 和其中一个一样的，形成新的中间节点
     *
     *          root_                       root_
     *           |                          /     \
     *          mid_2  _          ->      mid_2      mid_3  _
     *         /   \     \                 |  \        |      \
     *      leaf_1 leaf_2 leaf_3       leaf_1 leaf_3   leaf_2  new_leaf
     *
     * */
    cout << endl << "start of test 2" << endl;
    test = new TestTree();
    test->TestAdd(false, item1_shop3);
    test->TestAdd(false, item2_shop3);
    test->TestAdd(true, item3_shop3);
    test->TestAdd(true, item4_shop3);
    test->~TestTree();
    cout << "end of test 2" << endl << endl;
    /**
     * 如果最后一个节点有多个叶子，要保留祖先中的独支
     * (独支是一个或多个只有一个孩子的中间节点)
     *       ：                         ：
     *       ：                         ：
     *      mid_1                    mid_1
     *       |                        /   \
     *      mid_2         ->      leaf_1  mid_3
     *       |    \                        /   \
     *      leaf_1 leaf_2               leaf_2  new_leaf
     *
     * 用处：之后若加 和mid_1相同 但和mid_2不同的节点，能直接形成正确的树
     * 因为新的分支只能由*一个*叶子和新节点组成，如果没有保留mid1，就找不到
     * 正确的插入位置了
     *
     * 还需要检测如果leaf_2和新node会形成比leaf_1更高的子树，mid_1和mid_2
     * 会不会都被正确的消掉
     *
     * 如果叶子商品数有多个，若有独支也要删掉。因为只有一个节点，所以一定能
     * 形成正确的树
     * */
    cout << endl << "start of test 3" << endl;
    test = new TestTree();
    test->TestAdd(false, item13);
    test->TestAdd(false, item12);
    test->TestAdd(false, item12);
    test->TestAdd(false, item12);
    test->TestAdd(true, item11);
    test->TestAdd(true, item14);
    test->~TestTree();
    cout << "end of test 3" << endl << endl;

    /**
     * 添加或删除节点可能导致某个节点下最新的节点改变。
     * 比如
     *          |
     *          mid_1
     *          /  \
     *      leaf_1  leaf_2
     * 假如新节点和leaf_2形成了比mid_1更高的节点或者leaf_2直接被删掉了
     * mid_1在它sibling中的顺序可能就不对了。所以只能重新排序。
     *
     * 不过，我在京东app里测试的时候，如果删除的是同商店2物品中新的商品，旧的那个可以
     * 以正确的添加时间的顺序显示，
     * 但是如果删除的是同满减的2个物品中新的那个，旧的物品的顺序不会变，
     * 仍然以较新的那个商品的顺序显示....
     * */
    cout << endl << "start of test 4" << endl;
    test = new TestTree();
    test->TestAdd(false, item2_shop3);
    test->TestAdd(false, item14_no_discount);
    test->TestAdd(true, item1_shop3);
    test->TestAdd(true, item3);
    test->~TestTree();
    cout << "end of test 4" << endl << endl;

    /**
     * 形成新子树，剩下一个sibling的情况下，正确去除独支
     */
    cout << endl << "start of test 5" << endl;
    test = new TestTree();
    test->TestAdd(false, item1_shop3);
    test->TestAdd(false, item2_shop3);
    test->TestAdd(true, item15_no_discount);
    test->TestAdd(true, item3);
    test->~TestTree();
    cout << "end of test 5" << endl << endl;

    /**
     * 下面和的测试和删除有关
     */

    // 删除一个商品，只剩root_
    cout << endl << "start of test 7" << endl;
    test = new TestTree();
    test->TestAdd(false, item17_discount_2);   // A(a)
    test->TestAdd(false, item17_discount_2);   // A(a)
    test->TestAdd(false, item14_no_discount);  // B(b)
    test->TestDel(111, 17);
    test->TestDel(111, 17);
    test->TestDel(14, 14);
    test->~TestTree();
    cout << "end of test 7" << endl << endl;

    // 删除root_下几个商品中的一个
    cout << endl << "start of test 8" << endl;
    test = new TestTree();
    test->TestAdd(false, item14_no_discount);
    test->TestAdd(false, item15_no_discount);
    test->TestAdd(false, item16_no_discount);
    test->TestDel(15, 15);
    test->~TestTree();
    cout << "end of test 8" << endl << endl;

    // 删除有好几件相同商品中的一件
    // 减少商品件数不会影响展示顺序，和京东\淘宝一样
    cout << endl << "start of test 9" << endl;
    test = new TestTree();
    test->TestAdd(false, item14_no_discount);
    test->TestAdd(false, item15_no_discount);
    test->TestAdd(false, item16_no_discount);
    test->TestAdd(false, item15_no_discount);
    test->TestAdd(false, item16_no_discount);
    test->TestAdd(false, item15_no_discount);
    test->TestAdd(false, item16_no_discount);
    test->TestDel(15, 15);
    test->~TestTree();
    cout << "end of test 9" << endl << endl;

    // 删除某个有>=3个叶子的节点下的一个叶子
    cout << endl << "start of test 10" << endl;
    test = new TestTree();
    test->TestAdd(false, item20_no_discount);
    test->TestAdd(false, item21_no_discount);
    test->TestAdd(false, item22_no_discount);
    test->TestAdd(false, item23_no_discount);
    test->TestDel(1, 23);
    test->~TestTree();
    cout << "end of test 10" << endl << endl;

    // 删除某个有=2个叶子的节点下的一个叶子，无需重新排序

    // 删除某个有=2个叶子的节点下的一个叶子，需要重新排序
    /**
     *                root                               root
     *                  |                                   |
     *              mid_node1  _              (del leaf1)   mid_node1
     *           /      |         \              ->            |     \
     *      mid_node2  leaf3  mid_node3 (shop2)             mid_node3   \
     *    /(discount1)\         /       \                    /    |  \     leaf3
     *  leaf1      leaf2     leaf4    leaf5             leaf2  leaf4  leaf5
     *  (shop1)   (shop2)                        add time:  2      4    5     3
     */
    cout << endl << "start of test 6" << endl;
    test = new TestTree();
    test->TestAdd(false, item2);
    test->TestAdd(false, item1);
    test->TestAdd(false, item25_no_discount);
    test->TestAdd(false, item6);
    test->TestAdd(true, item23_no_discount);
    test->TestDel(2, 2);
    // test->ShowCart();
    test->~TestTree();
    cout << "end of test 6" << endl << endl;

    // 删除某个有=2个叶子的节点下的一个叶子，需要删除直接祖先的多个没有sibling的节点
    // 而且需要在停下之后进边上的子树
    /**
     *                root                              root
     *                  |                                 |
     *              mid_node1            (del leaf1)   mid_node1
     *           /           \              ->                 \
     *      mid_node2    mid_node3 (shop2)                     mid_node3
     *    /(discount1)\     /       \                           /    |   \
     *  leaf1      leaf2  leaf3    leaf4                    leaf3  leaf2  leaf4
     *  (shop1)   (shop2)                          add time:  1      3       4
     */
    cout << endl << "start of test 11" << endl;
    test = new TestTree();
    test->TestAdd(false, item20_no_discount);
    test->TestAdd(false, item2);
    test->TestAdd(false, item1);
    test->TestAdd(true, item23_no_discount);
    test->TestDel(2, 2);
    // test->ShowCart();
    test->~TestTree();
    cout << "end of test 11" << endl << endl;

    // 题目中的测试用例
    // // pj instruction中的测试用例1
    // cout << endl << "start of required test 1" << endl;
    // test = new TestTree();
    // test->TestAdd(false, item20_no_discount);  // A(a)
    // test->TestAdd(false, item21_no_discount);  // A(b)
    // test->TestAdd(false, item22_no_discount);  // B(c)
    // test->TestAdd(true, item23_no_discount);   // A(d)
    // test->~TestTree();
    // cout << "end of required test 1" << endl << endl;

    // // pj instruction中的测试用例2
    // cout << endl << "start of required test 2" << endl;
    // test = new TestTree();
    // test->TestAdd(false, item20_no_discount);  // A(a)
    // test->TestAdd(false, item22_no_discount);  // B(b)
    // test->TestAdd(true, item21_no_discount);   // A(c)
    // test->TestDel(1, 21);
    // test->~TestTree();
    // cout << "end of required test 2" << endl << endl;

    // // pj instruction中的测试用例3
    // cout << endl << "start of required test 3" << endl;
    // test = new TestTree();
    // test->TestAdd(false, item20_no_discount);  // A(a)
    // test->TestAdd(false, item22_no_discount);  // B(b)
    // test->TestAdd(true, item21_no_discount);   // C(c)
    // test->TestDel(1, 21);
    // test->~TestTree();
    // cout << "end of required test 3" << endl << endl;

    // // pj instruction中的测试用例4
    // cout << endl << "start of required test 4" << endl;
    // test = new TestTree();
    // test->TestAdd(false, item19_discount_2);  // A(a)
    // test->TestAdd(false, item18_discount_2);  // B(b)
    // test->TestAdd(true, item24_discount_2);   // C(c)
    // test->TestDel(333, 24);
    // test->~TestTree();
    // cout << "end of required test 4" << endl << endl;

    // // pj要求中的测试用例5：优先级
    // cout << endl << "start of required test 5" << endl;
    // test = new TestTree();
    // test->TestAdd(false, item17_discount_2);   // A(a)
    // test->TestAdd(false, item14_no_discount);  // B(b)
    // test->TestAdd(false, item18_discount_2);   // A(d)
    // test->TestAdd(true, item19_discount_2);    // C(c)
    // test->~TestTree();
    // cout << "end of required test 5" << endl << endl;
}

void TestPrintDiscount() {
    int ls[MAX_DISCOUNT_1] = {1};
    Item item(ls, ls);
    cout << item.PrintDiscount() << endl;
    return;
}

#endif