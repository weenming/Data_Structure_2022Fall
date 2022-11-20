#include <iostream>
#include <string>
#include <vector>

#include "item.hpp"
#include "test.hpp"
#include "tree.hpp"

using namespace std;

void TestPrintDiscount() {
    int ls[MAX_DISCOUNT_1] = {1};
    Item item(ls, ls);
    cout << item.PrintDiscount() << endl;
    return;
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

void TestFunc() {
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

    test->TestAdd(false, item12);
    test->TestAdd(false, item11);
    test->TestAdd(true, item14);
    test->~TestTree();
    cout << "end of test 1" << endl << endl;
    /* 加几个discount2 1-2形成一个中间节点(各个discount1 1-1不同)，然后再加一个
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
    test->TestAdd(false, item3_shop3);
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
    test->TestAdd(false, item11);
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
    test->TestAdd(false, item1_shop3);
    test->TestAdd(true, item3);
    test->~TestTree();
    cout << "end of test 5" << endl << endl;

    cout << endl << "start of test 5" << endl;
    test = new TestTree();
    test->TestAdd(false, item1_shop3);
    test->TestAdd(false, item2_shop3);
    test->TestAdd(false, item15_no_discount);
    test->TestAdd(true, item3);
    test->~TestTree();
    cout << "end of test 5" << endl << endl;

    // pj要求中的测试用例5：优先级
    cout << endl << "start of required test 5" << endl;
    test = new TestTree();
    test->TestAdd(false, item17_discount_2);   // A(a)
    test->TestAdd(false, item14_no_discount);  // B(b)
    test->TestAdd(false, item18_discount_2);   // A(d)
    test->TestAdd(true, item19_discount_2);    // C(c)
    test->~TestTree();
    cout << "end of required test 5" << endl << endl;

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
    test->TestAdd(false, item1);
    test->TestAdd(false, item2);
    test->TestAdd(false, item3);
    test->TestAdd(false, item4);
    test->TestAdd(false, item5);
    test->TestAdd(false, item6);
    test->TestAdd(true, item7);
    // test->TestDel(15, 15);
    test->~TestTree();
    cout << "end of test 10" << endl << endl;

    // 删除某个有=2个叶子的节点下的一个叶子，无需重新排序

    // 删除某个有=2个叶子的节点下的一个叶子，需要重新排序

    // 删除某个有=2个叶子的节点下的一个叶子，需要删除直接祖先的多个没有sibling的节点
    // 而且需要在停下之后

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
}

int main() {
    TestFunc();
    return 0;
}