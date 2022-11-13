#include <iostream>
#include <string>
#include <vector>

#include "item.hpp"
#include "tree.hpp"

using namespace std;

vector<string> MSG_1 = {"满2件打9折", "满3件打8折", "满4件打7折"};
vector<string> MSG_2 = {"满100元减10元", "满200元减40元", "满300元减100元"};

void TestPrintDiscount() {
    int ls[MAX_DISCOUNT_1] = {1};
    Item item(ls, ls);
    cout << item.PrintDiscount() << endl;
    return;
}

void TestAdd() {
    Item item1(0, 1, 1, 1);
    Item item2(0, 2, 1, 1);
    Item item3(0, 3, 0, 0);
    CartTree cart1;
    cart1.AddItem(&item1);

    cart1.AddItem(&item2);

    cart1.AddItem(&item3);
    cart1.ShowTree();
}

int main() {
    TestAdd();
    return 0;
}