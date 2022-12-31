#include <iostream>

#include "LoadData.hpp"

using namespace std;

void LookUp(UserTree* usr_tree, FileTree* file_tree, Permission* pms) {
    int msg;
    // 事实上，usr 48可以读file 0
    do {
        int usr_id, file_id;
        cout << "Type the user ID: (confirm by pressing enter)" << endl;
        cin >> usr_id;
        cout << "Type the file ID" << endl;
        cin >> file_id;
        cout << "Permission for " << usr_tree->GetName(usr_id) << " to access "
             << file_tree->GetName(file_id) << " is "
             << usr_tree->GetPermission(pms, usr_id, file_tree, file_id)
             << endl;
        cout << "Type 0 to exit, type others to look up another permission"
             << endl;
        cin >> msg;
    } while (msg);
}
void LoadData(UserTree* usr_tree, FileTree* file_tree, Permission* pms) {
    // load usr
    LoadTree("./USER_root", usr_tree, -1);
    // load file
    LoadTree("./FILE_root", file_tree, -1);
    // load pms
    LoadCsv(pms, usr_tree, file_tree, "./user_permission_file.csv");
}
int main() {
    UserTree* usr_tree = new UserTree();
    FileTree* file_tree = new FileTree();
    Permission* pms = new Permission();
    LoadData(usr_tree, file_tree, pms);
    LookUp(usr_tree, file_tree, pms);
    return 0;
}