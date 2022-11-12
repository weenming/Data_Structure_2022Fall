#include <iostream>
#include <string>
#include <vector>

#include "item.hpp"
#include "tree_node.hpp"

using namespace std;

string PrintDiscount(Item* item) {
    /* Notice: lower i, higher priority*/
    vector<string> msg_1 = {"满2件打9折", "满3件打8折", "满4件打7折"};
    vector<string> msg_2 = {"满100元减10元", "满200元减40元", "满300元减100元"};
    string discount_msgs;
    for (int i = 0; i < MAX_DISCOUNT_1 && i < msg_1.size(); i++) {
        if (item->discount_1[i]) {
            discount_msgs.append(msg_1[i]);
            discount_msgs.append(" ");
        }
    }
    for (int i = 0; i < MAX_DISCOUNT_2 && i < msg_2.size(); i++) {
        if (item->discount_2[i]) {
            discount_msgs.append(msg_2[i]);
            discount_msgs.append(" ");
        }
    }
    return discount_msgs;
}

void TestPrintDiscount() {
    bool ls[MAX_DISCOUNT_1] = {true, true, true, true, true,
                               true, true, true, true, true};
    Item item(ls, ls);
    cout << PrintDiscount(&item) << endl;
    return;
}

int main() {
    TestPrintDiscount();
    return 0;
}