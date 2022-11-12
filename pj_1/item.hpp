#include <iostream>
#include <string>
#define MAX_DISCOUNT_1 10
#define MAX_DISCOUNT_2 10
using namespace std;

struct Item {
    // one shop has one id
    int shop_id;
    /* Items in the same shop has different ids.
    shop_id and item_id together determine an item */
    int item_id;
    double price;
    /* Use 2 int arrays to store discount info. The first one
    is 跨店折扣, the value indexes the promotion, which is prior to store in
    sorting; while the second one is 跨店满减, which is minor to store in
    sorting. */
    bool discount_1[MAX_DISCOUNT_1];
    bool discount_2[MAX_DISCOUNT_2];
    /* First item to add into cart has add_time == 0,
    second item has 1 and so on. */
    int add_time;
    string name;
    string description;
    // Only 2 promotions in the whole store (折扣 & 满减 各 1 个)
    Item(int sid, int iid, double p, bool* d1, bool* d2, int adtime, string n)
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
    Item(bool* d1, bool* d2) {
        for (int i = 0; i < MAX_DISCOUNT_1; i++) {
            discount_1[i] = d1[i];
        }
        for (int i = 0; i < MAX_DISCOUNT_2; i++) {
            discount_2[i] = d2[i];
        }
    }
};
