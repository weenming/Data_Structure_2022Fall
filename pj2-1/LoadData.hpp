#include <io.h>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Load user tree and file tree from the structure of file directory
 */
template <class TreeCls>
TreeCls* LoadTree(string cur_path, TreeCls* tree, int parent_id) {
    // Recursively visit all directories in the current path
    _finddata_t FileInfo;

    if (cur_path) tree->AddItem(parent_id);
}

class LoadCsv {
   public:
    LoadCsv() {}
};