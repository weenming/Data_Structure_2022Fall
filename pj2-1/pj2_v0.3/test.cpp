#include <iostream>

#include "LoadData.hpp"
// #include "Permission.hpp"
// #include "Tree.hpp"

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

void LoadTreeTest() {
    // 116 users in total
    UserTree* usr_tree = new UserTree();
    LoadTree("./USER_root", usr_tree, -1);
    cout << "size of the usr tree:" << usr_tree->GetSize() << endl;
    cout << usr_tree->GetName(0) << ' ' << usr_tree->GetParent(0) << endl;
    cout << usr_tree->GetName(1) << ' ' << usr_tree->GetParent(1) << endl;
    cout << usr_tree->GetName(usr_tree->GetSize() - 1) << ' '
         << usr_tree->GetParent(usr_tree->GetSize() - 1) << endl;

    // 529 files in total
    FileTree* file_tree = new FileTree();
    LoadTree("./FILE_root", file_tree, -1);
    cout << "size of the usr tree:" << file_tree->GetSize() << endl;
    cout << file_tree->GetName(0) << ' ' << file_tree->GetParent(0) << endl;
    cout << file_tree->GetName(1) << ' ' << file_tree->GetParent(1) << endl;
    cout << file_tree->GetName(file_tree->GetSize() - 1) << ' '
         << file_tree->GetParent(file_tree->GetSize() - 1) << endl;
}

void LoadPmsTest() {
    // load usr
    UserTree* usr_tree = new UserTree();
    LoadTree("./USER_root", usr_tree, -1);
    // load file
    FileTree* file_tree = new FileTree();
    LoadTree("./FILE_root", file_tree, -1);

    // load pms
    Permission* pms = new Permission();
    LoadCsv(pms, usr_tree, file_tree, "./user_permission_file.csv");
    cout << pms->GetPms(10, 100) << endl;
}

void AllTest() {
    // load usr
    UserTree* usr_tree = new UserTree();
    LoadTree("./USER_root", usr_tree, -1);
    // load file
    FileTree* file_tree = new FileTree();
    LoadTree("./FILE_root", file_tree, -1);

    // load pms
    Permission* pms = new Permission();
    LoadCsv(pms, usr_tree, file_tree, "./user_permission_file.csv");
    // usr: gloom, file: distort
    cout << pms->GetPms(48, 0) << endl;
    cout << usr_tree->GetPermission(pms, 48, file_tree, 0) << endl;
    cout << "User name:" << usr_tree->GetName(48)
         << ", file name: " << file_tree->GetName(0) << endl;
}

void myTest() {
    /**
     * 几种情况：
     * 1. File 没有父节点
     *  1. Usr没有父节点
     *  2. Usr有父节点但是permission并不更低
     *  3. Usr有父节点且Permission更低
     * 2. File有父节点且Permission不更低
     *  1. Usr没有父节点
     *  2. Usr有父节点但Permission并不更低
     *  3. Usr有父节点且Permission更低
     * 3. File有父节点且Permission更低
     *  1. Usr没有父节点
     *  2. Usr有父节点但Permission并不更低
     *  3. Usr有父节点且Permission更低
     * 4. File不存在或者Usr不存在
     */

    // 1.1
    FileTree* f = new FileTree();
    UserTree* u = new UserTree();
    Permission* p = new Permission();
    f->AddItem(-1);
    u->AddItem(-1);
    p->AddPms(0, 0, 2);
    cout << u->GetPermission(p, 0, f, 0) << " should be 2" << endl;

    // 1.2
    f = new FileTree();
    u = new UserTree();
    p = new Permission();
    f->AddItem(-1);
    u->AddItem(-1);
    u->AddItem(0);
    p->AddPms(0, 0, 1);
    p->AddPms(1, 0, 2);
    cout << u->GetPermission(p, 1, f, 0) << " should be 1" << endl;

    // 1.3
}

int main() {
    AllTest();
    return 0;
}