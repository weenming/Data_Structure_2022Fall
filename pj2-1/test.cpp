#include <iostream>

#include "Permission.hpp"
#include "Tree.hpp"
using namespace std;

void TestPms() {
    Permission* p = new Permission();
    p->AddPms(0, 0, 1);
    p->AddPms(0, 1, 2);
    p->AddPms(0, 2, 0);
    p->AddPms(0, 3, 1);
    p->AddPms(0, 4, 2);
    p->AddPms(0, 400, 2);
    p->AddPms(0, 4000, 1);

    cout << p->GetPms(0, 10) << " should be 0" << endl;
    cout << p->GetPms(0, 100) << " should be 0" << endl;
    cout << p->GetPms(1, 0) << " should be 0" << endl;
    cout << p->GetPms(1, 10) << " should be 0" << endl;
    cout << p->GetPms(100, 0) << " should be 0" << endl;
    cout << p->GetPms(100, 10) << " should be 0" << endl;

    cout << p->GetPms(0, 0) << " should be 1" << endl;
    cout << p->GetPms(0, 2) << " should be 0" << endl;
    cout << p->GetPms(0, 4) << " should be 2" << endl;
    cout << p->GetPms(0, 400) << " should be 2" << endl;
    cout << p->GetPms(0, 4000) << " should be 1" << endl;

    p->AddPms(1, 1, 1);
    p->AddPms(200, 1, 2);
    p->AddPms(1, 1, 2);

    cout << p->GetPms(1, 1) << " should be 1" << endl;
    cout << p->GetPms(200, 1) << " should be 2" << endl;
    cout << p->GetPms(1, 1) << " should be 2" << endl;
}

void TestUsr() {
    // no parent dependence
    Permission* pms = new Permission();
    pms->AddPms(0, 0, 1);
    pms->AddPms(0, 1, 2);
    pms->AddPms(0, 2, 0);

    pms->AddPms(1, 0, 2);
    pms->AddPms(1, 1, 1);
    pms->AddPms(1, 2, 0);

    pms->AddPms(2, 2, 1);

    FileTree* ftree = new FileTree();
    ftree->AddItem(-1);  // file 0
    ftree->AddItem(-1);  // file 1
    ftree->AddItem(-1);  // file 2

    UserTree* usr_tree = new UserTree();
    usr_tree->AddItem(-1);  // usr_id == 0, parent == -1: root
    usr_tree->AddItem(-1);  // usr 1
    usr_tree->AddItem(-1);  // usr 2
    usr_tree->AddItem(-1);  // usr 3

    cout << usr_tree->GetPermission(pms, 0, ftree, 0) << "should be 1" << endl;
    cout << usr_tree->GetPermission(pms, 0, ftree, 1) << "should be 2" << endl;
    cout << usr_tree->GetPermission(pms, 0, ftree, 2) << "should be 0" << endl;

    cout << usr_tree->GetPermission(pms, 1, ftree, 0) << "should be 2" << endl;
    cout << usr_tree->GetPermission(pms, 1, ftree, 1) << "should be 1" << endl;
    cout << usr_tree->GetPermission(pms, 1, ftree, 2) << "should be 0" << endl;

    cout << usr_tree->GetPermission(pms, 2, ftree, 0) << "should be 0" << endl;
    cout << usr_tree->GetPermission(pms, 2, ftree, 1) << "should be 0" << endl;
    cout << usr_tree->GetPermission(pms, 2, ftree, 2) << "should be 1" << endl;
}

int main() {
    TestUsr();
    return 0;
}