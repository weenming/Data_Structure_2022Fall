#include <assert.h>

#include <unordered_map>
#include <vector>

using namespace std;

extern const int MAX_PERMISSION_LEVEL = 2;

class Tree {
    // 用双亲表示
   protected:
    // 储存parent的id，也就是数组下标
    vector<int> _parent;
    bool _HasParent(int id) { return id >= 0; }
    int GetParent(int id) { return _parent[id]; }

   public:
    Tree() { _parent = vector<int>(); }
    int AddItem(int parent_id) {
        // Ah,I think _parent.size() is an unsigned number... That should be why
        // -1 > 1...

        assert(parent_id < 0 || parent_id < _parent.size());
        _parent.push_back(parent_id);
        // 返回新加进去的元素的id
        return _parent.size() - 1;
    }
};

class FileTree : public Tree {
    friend class UserTree;
};

class UserTree : public Tree {
   public:
    int GetPermission(Permission* pms, int usr_id, FileTree* ftree,
                      int file_id);
};

int UserTree::GetPermission(Permission* pms, int usr_id, FileTree* ftree,
                            int file_id) {
    int least_pms = MAX_PERMISSION_LEVEL + 1;
    while (_HasParent(usr_id)) {
        while (ftree->_HasParent(file_id)) {
            int this_pms = pms->GetPms(usr_id, file_id);
            // 如果没权限，看都不用看了
            if (this_pms == 0) return 0;
            if (least_pms > this_pms) {
                least_pms = this_pms;
            }
            // 开始看父文件
            file_id = ftree->GetParent(file_id);
        }
        // 遍历这个用户完成，看父用户
        usr_id = GetParent(usr_id);
    }
    return least_pms;
}
