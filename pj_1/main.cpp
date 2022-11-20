#include <iostream>
#include <string>
#include <vector>

#include "item.hpp"
#include "test.hpp"
#include "tree.hpp"

using namespace std;

void TestCases(int n) {
    CartTree* tree = new CartTree();
    Item* item1;
    Item* item2;
    Item* item3;
    Item* item4;
    // shop_id, item_id, price, discount_1[0], discount_2[0], name
    // 我们是
    // Example 1
    switch (n) {
        case 1:
            item1 = new Item(0, 1, 99.00, 0, 0, "a");
            item2 = new Item(0, 2, 88.00, 0, 0, "b");
            item3 = new Item(1, 3, 99.00, 0, 0, "c");
            item4 = new Item(0, 4, 77.00, 0, 0, "d");
            tree->ShowCart();
            tree->AddItem(item1);
            tree->ShowCart();
            tree->AddItem(item2);
            tree->ShowCart();
            tree->AddItem(item3);
            tree->ShowCart();
            tree->AddItem(item4);
            tree->ShowCart();
            tree->ShowTreePretty(tree->get_root(), "");
            tree->~CartTree();
            break;
        case 2:
            item1 = new Item(0, 1, 99.00, 0, 0, "a");
            item2 = new Item(1, 2, 88.00, 0, 0, "b");
            item3 = new Item(0, 3, 99.00, 0, 0, "c");
            tree->ShowCart();
            tree->AddItem(item1);
            tree->ShowCart();
            tree->AddItem(item2);
            tree->ShowCart();
            tree->AddItem(item3);
            tree->ShowCart();
            // CartTree::DelItem(int shop_id, int item_id)
            tree->DelItem(0, 3);
            tree->ShowCart();
            tree->ShowTreePretty(tree->get_root(), "");
            tree->~CartTree();
            break;

        case 3:
            item1 = new Item(0, 1, 99.00, 0, 1, "a");
            item2 = new Item(1, 2, 88.00, 0, 0, "b");
            item3 = new Item(2, 3, 77.00, 0, 1, "c");
            tree->ShowCart();
            tree->AddItem(item1);
            tree->ShowCart();
            tree->AddItem(item2);
            tree->ShowCart();
            tree->AddItem(item3);
            tree->ShowCart();
            // CartTree::DelItem(int shop_id, int item_id)
            tree->DelItem(2, 3);
            tree->ShowCart();
            tree->ShowTreePretty(tree->get_root(), "");
            tree->~CartTree();
            break;

        case 4:
            item1 = new Item(0, 1, 99.00, 1, 0, "a");
            item2 = new Item(1, 2, 88.00, 0, 0, "b");
            item3 = new Item(2, 3, 77.00, 1, 0, "c");
            tree->ShowCart();
            tree->AddItem(item1);
            tree->ShowCart();
            tree->AddItem(item2);
            tree->ShowCart();
            tree->AddItem(item3);
            tree->ShowCart();
            // CartTree::DelItem(int shop_id, int item_id)
            tree->DelItem(2, 3);
            tree->ShowCart();
            tree->ShowTreePretty(tree->get_root(), "");
            tree->~CartTree();
            break;

        case 5:
            item1 = new Item(0, 1, 99.00, 0, 1, "a");
            item2 = new Item(1, 2, 88.00, 0, 0, "b");
            item3 = new Item(2, 3, 77.00, 0, 1, "c");
            item4 = new Item(0, 4, 66.00, 0, 0, "d");
            tree->ShowCart();
            tree->AddItem(item1);
            tree->ShowCart();
            tree->AddItem(item2);
            tree->ShowCart();
            tree->AddItem(item4);
            tree->ShowCart();
            tree->AddItem(item3);
            tree->ShowCart();
            tree->ShowTreePretty(tree->get_root(), "");
            tree->~CartTree();
            break;

        default:
            break;
    }
}

int main() {
    // 额外的10个测试
    TestAddAndDel();
    // 题目里的5个测试
    // TestCases(5);
    return 0;
}