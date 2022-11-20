#ifndef ITEM_H_
#define ITEM_H_

#include <cassert>
#include <iostream>
#include <string>

#include "globals.hpp"
using namespace std;

extern vector<vector<string>> MSG_1;
extern vector<vector<string>> MSG_2;
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
    string name;
    string description;
    // Only 2 promotions in the whole store (折扣 & 满减 各 1 个)
    Item(int sid, int iid, double p, int d1, int d2, string n)
        : shop_id(sid),
          item_id(iid),
          price(p),
          discount_1{d1},
          discount_2{d2},
          name(n){};
    Item(int sid, int iid, double p, string n)
        : shop_id(sid),
          item_id(iid),
          price(p),
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
        for (int i = 1; i < MAX_DISCOUNT_1; i++) discount_1[i] = 0;
        for (int i = 1; i < MAX_DISCOUNT_2; i++) discount_2[i] = 0;
        item_id = iid;
        shop_id = sid;
    }

    string PrintDiscount();
    void ShowItem();
    void ShowItem(string);
};

string Item::PrintDiscount() {
    /* Notice: lower i, higher priority
    MSG1 有 MAX_DISCOUNT_1 个vectors of strings, the string in the i-thh vector
    is indexed with discount_1[i]
    */

    string discount_msgs;
    for (int i = 0; i < MAX_DISCOUNT_1 && i < MSG_1.size(); i++) {
        if (discount_1[i]) {
            assert(MSG_1[i].size() > discount_1[i] - 1);
            // discount_1[i] 从1开始，但是vector的index从0开始
            discount_msgs.append(MSG_1[i][discount_1[i] - 1]);
            discount_msgs.append(" ");
        }
    }
    for (int i = 0; i < MAX_DISCOUNT_2 && i < MSG_2.size(); i++) {
        if (discount_2[i]) {
            assert(MSG_2[i].size() > discount_2[i] - 1);
            // discount_1[i] 从1开始，但是vector的index从0开始
            discount_msgs.append(MSG_2[i][discount_2[i] - 1]);
            discount_msgs.append(" ");
        }
    }
    return discount_msgs;
}

void Item::ShowItem() {
    cout << "item id: " << item_id << endl;
    cout << "shop id: " << shop_id << endl;

    cout << PrintDiscount() << endl;
    return;
}

void Item::ShowItem(string indent) {
    cout << indent << "item id:" << item_id << endl;
    cout << indent << "shop id: " << shop_id << endl;

    cout << indent << PrintDiscount() << endl;
    return;
}

#endif