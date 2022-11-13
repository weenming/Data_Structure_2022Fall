#ifndef ITEM_H_
#define ITEM_H_

#include <iostream>
#include <string>
#define MAX_DISCOUNT_1 1
#define MAX_DISCOUNT_2 1
using namespace std;

extern vector<string> MSG_1;
extern vector<string> MSG_2;

struct Item {
    // one shop has one id
    int shop_id;
    /* Items in the same shop has different ids.
    shop_id and item_id together determine an item */
    int item_id;
    double price;
    /* Use 2 int arrays to store discount info. The first one
    is 跨店折扣, discount_1[i] == j means the item is on 第j种折扣，with
    优先级i, which is always prior to store in sorting; while the second one is
    跨店满减, which is minor to store in sorting. */
    int discount_1[MAX_DISCOUNT_1];
    int discount_2[MAX_DISCOUNT_2];
    /* First item to add into cart has add_time == 0,
    second item has 1 and so on. */
    int add_time;
    string name;
    string description;
    // Only 2 promotions in the whole store (折扣 & 满减 各 1 个)
    Item(int sid, int iid, double p, int d1, int d2, int adtime, string n)
        : shop_id(sid),
          item_id(iid),
          price(p),
          discount_1{d1},
          discount_2{d2},
          add_time(adtime),
          name(n){};
    Item(int sid, int iid, double p, int adtime, string n)
        : shop_id(sid),
          item_id(iid),
          price(p),
          add_time(adtime),
          name(n),
          discount_1{false},
          discount_2{false} {}
    // For test: discount info
    Item(int* d1, int* d2) {
        for (int i = 0; i < MAX_DISCOUNT_1; i++) {
            discount_1[i] = d1[i];
        }
        for (int i = 0; i < MAX_DISCOUNT_2; i++) {
            discount_2[i] = d2[i];
        }
    }
    // For test: add item
    Item(int sid, int iid, int d1, int d2) {
        discount_1[0] = d1;
        discount_2[0] = d2;
        item_id = iid;
        shop_id = sid;
    }

    string PrintDiscount();
    void ShowItem();
};

string Item::PrintDiscount() {
    /* Notice: lower i, higher priority*/

    string discount_msgs;
    for (int i = 0; i < MAX_DISCOUNT_1 && i < MSG_1.size(); i++) {
        if (discount_1[i]) {
            discount_msgs.append(MSG_1[i]);
            discount_msgs.append(" ");
        }
    }
    for (int i = 0; i < MAX_DISCOUNT_2 && i < MSG_2.size(); i++) {
        if (discount_2[i]) {
            discount_msgs.append(MSG_2[i]);
            discount_msgs.append(" ");
        }
    }
    return discount_msgs;
}

void Item::ShowItem() {
    cout << "shop id: " << shop_id << endl;
    cout << "item id:" << item_id << endl;
    cout << PrintDiscount() << endl << endl;
    return;
}

#endif