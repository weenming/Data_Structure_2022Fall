#include <assert.h>

#include <unordered_map>
#include <vector>

using namespace std;

extern const int MAX_PERMISSION_LEVEL = 2;
extern const int MIN_PMS_LEVEL = 0;

class Tree {
    // 用双亲表示
   protected:
    // 储存parent的id，也就是数组下标
    vector<int> _parent;
    // 储存name
    vector<string> _names;
    unordered_map<string, int> _ids;
    bool _HasParent(int id) { return id >= 0; }

   public:
    int GetParent(int id) {
        assert(id >= 0 && id < _parent.size());
        return _parent[id];
    }
    string GetName(int id) {
        assert(id >= 0 && id < _names.size());
        return _names[id];
    }
    Tree() {
        _parent = vector<int>();
        _names = vector<string>();
        _ids = unordered_map<string, int>();
    }
    int AddItem(int);
    int AddItem(int, string);
    int GetSize() { return _parent.size(); }
    int GetId(string name) {
        auto found = _ids.find(name);
        assert(found != _ids.end());  // otherwise name not loaded yet;
        return found->second;
    }
};

// Only for test
int Tree::AddItem(int parent_id) {
    assert(parent_id < 0 || parent_id < _parent.size());
    _parent.push_back(parent_id);
    _names.push_back(string());
    return _parent.size() - 1;
}

int Tree::AddItem(int parent_id, string name) {
    // Ah,I think _parent.size() is an unsigned number... That should be why
    // -1 > 1...
    // 根节点的parent是-1，其实这个类是个森林？
    assert(parent_id < 0 || parent_id < _parent.size());
    _parent.push_back(parent_id);
    _names.push_back(name);
    // Add new elem in hash map, key: name, val: id
    auto found = _ids.find(name);
    // must be new node
    assert(found == _ids.end());
    // might overflow?
    int new_id = _parent.size() - 1;
    _ids.insert({name, new_id});
    // 返回新加进去的元素的id
    return new_id;
}

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
    while (this->_HasParent(usr_id)) {
        int cur_fid = file_id;  // 对每个usr都要遍历file
        while (ftree->_HasParent(cur_fid)) {
            int this_pms = pms->GetPms(usr_id, cur_fid);
            // 如果没权限，看都不用看了
            if (this_pms == MIN_PMS_LEVEL) return MIN_PMS_LEVEL;
            if (least_pms > this_pms) {
                least_pms = this_pms;
            }
            // 开始看父文件
            cur_fid = ftree->GetParent(cur_fid);
        }
        // 遍历这个用户完成，看父用户
        usr_id = GetParent(usr_id);
    }
    if (least_pms <= MAX_PERMISSION_LEVEL)
        return least_pms;
    else  // when no permission record in ancestors
        return MIN_PMS_LEVEL;
}
