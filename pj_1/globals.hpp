#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <string>
#include <vector>
using namespace std;

// 最大的不同优先级的折扣数量
#define MAX_DISCOUNT_1 10
// 最大的不同优先级的折扣数量
#define MAX_DISCOUNT_2 10
// 购物车中最大的商品数
#define MAX_ITEM_NUMBER 100

// 整个商城中正在进行的活动

// 一个商品不能有同一优先级的几个折扣，要不就不知道放哪了
// 这等价于，任意节点开始的祖先链条中，不会有相同优先级的不同折扣/商店/满减
// 折扣里优先级最高的折扣们
vector<string> MSG_1_1 = {"折扣1-a", "折扣1-b", "折扣1-c"};
// 折扣里优先级第二的
vector<string> MSG_1_2 = {"折扣2-a", "折扣2-b", "折扣2-c"};
vector<string> MSG_1_3 = {"折扣3-a", "折扣3-b", "折扣3-c"};
vector<vector<string>> MSG_1 = {MSG_1_1, MSG_1_2, MSG_1_3};
// 满减里优先级最高的
vector<string> MSG_2_1 = {"满减1-a", "满减1-b", "满减1-c"};
vector<string> MSG_2_2 = {"满减2-a", "满减2-b", "满减2-c"};
vector<string> MSG_2_3 = {"满减3-a", "满减3-b", "满减3-c"};
vector<vector<string>> MSG_2 = {MSG_2_1, MSG_2_2, MSG_2_3};

vector<string> SHOP_NAME = {"商店Alpha", "商店Beta", "商店Gamma", "商店Delta"};

#endif