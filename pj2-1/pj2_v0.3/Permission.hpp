#include <assert.h>

#include <unordered_map>
#include <vector>

using namespace std;

class Permission {
    // 储存权限信息，用嵌套的hash table实现
   private:
    // first key is: usr id; the nested hashmap's key is file_id
    unordered_map<int, unordered_map<int, int>*>* _pms_hash_map;
    bool _HasUsrKey(int usr) {
        return _pms_hash_map->find(usr) != _pms_hash_map->end();
    }

    bool _HasFileKey(int usr, int file) {
        assert(_HasUsrKey(usr));
        auto inner_map = _pms_hash_map->find(usr)->second;
        return inner_map->find(file) != inner_map->end();
    }

   public:
    Permission() {
        _pms_hash_map = new unordered_map<int, unordered_map<int, int>*>;
    }
    void AddPms(int, int, int);
    int GetPms(int usr_id, int file_id);
};

void Permission::AddPms(int usr_id, int file_id, int level) {
    unordered_map<int, int>* nested_map;
    if (!_HasUsrKey(usr_id)) {
        //
        nested_map = new unordered_map<int, int>;
        _pms_hash_map->insert({usr_id, nested_map});
    } else {
        nested_map = _pms_hash_map->find(usr_id)->second;
    }
    // 应该不会出现两个相同key的情况，但如果出现了，只会保留第一次定义的
    nested_map->insert({file_id, level});
}

int Permission::GetPms(int usr_id, int file_id) {
    if (!_HasUsrKey(usr_id)) {  // 查找1次
        // no usr info in permission list
        return 0;
    } else if (!_HasFileKey(usr_id,
                            file_id)) {  // 查找2次(assert 里多了一次UsrKey检查)
        // no file info for this usr
        return 0;
    } else {
        return _pms_hash_map->find(usr_id)->second->find(file_id)->second;
    }
}
