#ifndef TREE_H_
#define TREE_H_

#include <cassert>
#include <iostream>
#include <string>

#include "item.hpp"

using namespace std;

struct TreeNode {
    // left: son, right:sibling
    TreeNode* sibling;
    TreeNode* son;
    TreeNode* parent;
    Item* item;
    int data;
    // only leaves have non-zero item number...
    int number;
    int add_time;
    TreeNode() {
        // for initializing mid nodes
        item = nullptr;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
        number = 0;
    }
    TreeNode(Item* n, int time) {
        // for initializing leaves
        item = n;
        sibling = nullptr;
        son = nullptr;
        data = -1;
        parent = nullptr;
        number = 1;
        add_time = time;
    }
};

class CartTree {
   protected:
    TreeNode* root_;
    int __SubTreePriority(TreeNode*, Item*, TreeNode*);
    void __ShowCartRecur(TreeNode*);
    void __DelTree(TreeNode*);
    void __AddMidNode(TreeNode*, Item*);
    bool __NoAncestor(int, TreeNode*);
    TreeNode* __get_prev_sibling(TreeNode*);
    void __DelBadNodes(TreeNode*);
    void __UpdateMidNodes(int, TreeNode*);
    void __SortAncestors(TreeNode*);
    bool __SamePriority(TreeNode*, Item*);
    int last_time_;

   public:
    CartTree();
    ~CartTree() { __DelTree(root_); };
    void AddItem(Item*);
    TreeNode* get_root();
    // delete: ref with ptr
    void DelItem(Item*);
    // delete: ref with index
    void DelItem(int shop_id, int item_id);
    // delete: ref with name
    void DelItem(string name);
    void ShowCart() {
        cout << "Showing Shopping cart:\n" << endl;
        __ShowCartRecur(root_);
    };
    void ShowTreePretty(TreeNode*, string);
};

void CartTree::ShowTreePretty(TreeNode* cur, string indent) {
    if (cur == nullptr) {
        return;
    }
    if (cur->data != -1) {
        cout << indent << "|- Node: data = " << cur->data << endl;
    } else if (cur->item != nullptr) {
        // leaf
        cout << indent << "|- Leaf " << endl;
        cur->item->ShowItem(indent + "*  ");
    } else {
        // cur->item == nullptr: should be root
        cout << indent << "|- Root " << endl;
    }
    ShowTreePretty(cur->son, indent + "*    ");
    ShowTreePretty(cur->sibling, indent);
}

TreeNode* CartTree::get_root() { return root_; }

CartTree::CartTree() {
    TreeNode* root_node = new TreeNode();
    root_ = root_node;
    last_time_ = 0;
    return;
}

void CartTree::__DelTree(TreeNode* node) {
    if (node != nullptr) {
        __DelTree(node->sibling);
        __DelTree(node->son);
        delete node;
    }
    return;
}

void CartTree::__ShowCartRecur(TreeNode* cur) {
    // 为啥debug的时候，这里没有加一行cout的话就不会报exception
    if (cur == nullptr) return;

    // cout << cur << endl;
    if (cur->son != nullptr) {
        __ShowCartRecur(cur->son);
    } else {
        cur->item->ShowItem();
        cout << "item number: " << cur->number << endl << endl;
    }
    __ShowCartRecur(cur->sibling);
}

void CartTree::AddItem(Item* item) {
    /*
     * The tree is arranged as described here.
     * FROM BOTTOM TO TOP:
     * Leaves: all items in the cart
     *   data: -1
     *
     * 1-th layer parents:
     *   item: ptr to the descendant's item with the most recent add_time
     *   data: an int determining sorting value (e.g., item->discount_1[0](where
     *   data == 0), (where data == MAX_DISCOUNT_1), item->discount_2[3] (where
     *   data == MAX_DISCOUNT_1 + 4), etc.).
     *
     *   The descendants of a node in this layer are sorted  according to least
     *   prior sorting condition, i.e. "跨店满减". From left to right, the nodes
     *   are stored in ascending order of item->add_time
     *
     * 2-th layer:
     *   item: same
     *   data: same
     *   The descendants of a node in this layer are sorted  according to second
     *   least prior sorting condition, i.e. another "跨店满减" (if there are
     *   multiple ones), or the next rule, "Same-Store". From left to right, the
     *   nodes are stored in ascending order of item->add_time
     *
     * 3-th layer...
     *
     * root: empty, item == nullptr


    在寻找添加位置时，从上到下寻找。选择访问节点A的哪个子节点(B1, B2,
    B3...)，按照下面的规则
    a.
    如果Bn是叶子节点，返回Bn和item相同且和A的所有祖先节点对应优先级不同的最高优先级

    b.
    如果Bn不是叶子节点，
        b1. 如果Bn对应的优先级和item一样
            返回Bn的子节点中的最高优先级的节点对应的最高优先级
        b2. 如果不一样，返回最低的优先级(MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1)

    有这样几种情况：
        some_node --- with same discount_1[1] as item to add
        |
        |
        cur --- with same discount_2[0] as item to add
       /    \      \
    item2    item3   \
    (shop2)  (shop3)   some_other_node (same discount_1[2]) as item to add
                        \
                        item4 (shop4)
    (1) when item to add: item->shop_id != 2 and item->shop_id != 3,
    item should be added as a direct leaf of cur.

    (2) cur节点的某个非叶子节点下的叶子有和item相同的包含在祖先里的最高优先级

    (3) cur节点的某个非叶子节点下的叶子有和item相同的不包含在祖先里的最高优先级
        只有这个叶子节点对应的优先级和item相同才能访问这个节点。之后要增加一个中
        间节点

    (4)
    可能某个cur节点下面有直接叶子，拥有和item相同的不包括在祖先里的相同优先级，
    就要访问这个叶子。之后要增加一个中间节点

    when item to add: item->shop_id == 2, a new node should be
    created
            some_node --- with same discount_1[0] as item to add
                |       \
                |       ...
                cur --- with same discount_2[0] as item to add
                /    \
            new_node    \
            /    \         \
        item2   new_item    item3
        (shop2)  (shop2)      (shop3)
        In fact the new node may have higher priority, and thus should
        be added to the preceding positions in the tree. This does not
        matter because a backward search will be executed afterwards.

    (5) when item to add has the same shop_id and item_id, they are
        considered an identical item. Visit the next node and it can be
        eventually handled.
    这时想要的leaf和item的相同且不含祖先的优先级一定是最高的，按照前面的方法
    走到最后一定能到想要的叶子

    之后
    访问到某个叶子，且这个newnode+这个叶子一定能形成比它的parent更高的优先级的时候
    判断这个叶子是不是新item一样的商品。如果不是，就根据对应的相同新优先级形成新的mid
    nodes

    (如果这个叶子没有和newnode一样的新优先级，那上次while中应该就已经把newnode接上，成为叶子的sibling了)

    先造出来正确的新中间节点
        mid_node_1 --- 和item相同且不包含在祖先里的最高优先级
            |
            :
            :
            |
        mid_node_n --- 第n高的优先级
    -------先造上面，再造下面------
          /   \
         /     \
       root    node_to_add (construct with item)


    */

    // Now, from top to bottom, traverse the tree. Look for subtrees to add the
    // new node. If a certain subtree is to be inserted, item of this node
    // points to the latest item.
    // 更新加入时间
    last_time_ += 1;
    // find place to insert
    // 第一次添加
    if (root_->son == nullptr) {
        TreeNode* new_node = new TreeNode(item, last_time_);
        new_node->parent = root_;
        root_->son = new_node;
        return;
    }
    // 不是第一次添加
    TreeNode* root = root_;
    // 可能高优先级的节点在低优先级的子树下...至少O(N)了这下
    TreeNode *cur, *cur_parent, *next_root;
    int this_priority, best_priority;
    // 找到最高优先级的那个叶子节点，插入新节点
    // 或者在寻找过程中，已经不可能存在new node
    // 和某个叶子节点组成的，不包含在祖先里的新节点。
    while (root->son != nullptr) {
        best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
        next_root = nullptr;
        cur = root->son;
        // 遍历root下的所有孩子，找到最高优先级的那个孩子
        while (cur != nullptr) {
            /* SubTreePriority 返回的是cur的各个叶子中，和item相同的且不包含在
               cur的祖先里的最高的相同优先级。不过如果不能进cur，cur下比cur低的
               优先级是不算的
             */
            this_priority = __SubTreePriority(cur, item, cur);
            if (best_priority > this_priority) {
                // this is the next root
                best_priority = this_priority;
                next_root = cur;
            }
            cur = cur->sibling;
        }
        // 第一种情况：如果一个相同discount都没有，加到root的最左边就行
        if (!next_root) {
            // 但还是有可能是同种商品：可能不同商品折扣完全一样而且是同一家店
            // subtreepriority不能分辨出出来...
            cur = root->son;
            while (cur) {
                if (cur->data == -1 && cur->item->shop_id == item->shop_id &&
                    cur->item->item_id == item->item_id) {
                    root = cur;
                    break;
                }
                cur = cur->sibling;
            }
            if (cur == root) break;

            TreeNode* new_node = new TreeNode(item, last_time_);
            new_node->sibling = root->son;
            new_node->parent = root;
            root->son = new_node;
            // 添加完成了
            return;
        }
        // 第二种情况：最好的是叶子
        else if (next_root->son == nullptr) {
            // 把叶子移到最前面
            cur = __get_prev_sibling(next_root);
            if (cur != nullptr) {
                cur->sibling = next_root->sibling;
                next_root->sibling = root->son;
                root->son = next_root;
            }
            // ShowTreePretty(root_, "");
            // 到下一层节点(叶子)去
            root = next_root;
            break;
        }
        // 第三种情况：最好的是中间节点
        else if (__SamePriority(next_root, item)) {
            // 如果确实能访问这个节点，要更新
            // 由于新加的节点是最新的，所以把next root移到siblings中的最左边
            // 如果nextroot不是最左孩子，维护sibling指针
            cur = __get_prev_sibling(next_root);
            if (cur != nullptr) {
                cur->sibling = next_root->sibling;
                next_root->sibling = root->son;
                root->son = next_root;
            }
            // 更新item
            next_root->item = item;
            next_root->add_time = last_time_;
            // 到下一层节点去
            root = next_root;
        }
        // 第四种情况：最好的是中间节点但是不要更新中间节点
        else {
            // 因为将要形成更高优先级的子树才到下个节点，不应该更新这个节点
            // 不过到叶子节点的那一次还是可以用之前的，因为重新排序将要形成新子树
            // 的那个节点没影响。
            root = next_root;
        }
    }

    if (root->item->shop_id == item->shop_id &&
        root->item->item_id == item->item_id) {  // 一样的商品
        root->number += 1;
        root->add_time = last_time_;
    } else {
        __AddMidNode(root, item);
    }
    return;
}

void CartTree::__AddMidNode(TreeNode* root, Item* item) {
    assert(root != nullptr && root->parent != nullptr);
    // 先造出来新的中间节点， 需要找到不包含在祖先中的所有相同优先级
    // 逻辑是把高优先级的先插进去，然后循环调用SubTreePriority 就行
    int best_priority, cur_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    TreeNode *first_mid_node = root->parent, *last_mid_node;
    assert(first_mid_node->son == root);
    /*
            (original tree)
                  |
             first_mid_node
                  |  \
                  :   \
                  :   siblings of root in the original tree
                  :
                  |
            last_mid_node
               /    \
           root   (new_node, not yet added)
           多了一个优先级21的节点。。。
    */
    cur_priority = __SubTreePriority(root, item, root);
    assert(cur_priority != MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1);
    best_priority = cur_priority;
    while (__SubTreePriority(root, item, root) !=
           MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1) {
        cur_priority = __SubTreePriority(root, item, root);
        last_mid_node = new TreeNode();
        last_mid_node->item = item;
        last_mid_node->add_time = last_time_;
        last_mid_node->data = cur_priority;
        last_mid_node->parent = root->parent;
        last_mid_node->son = root;
        root->parent->son = last_mid_node;
        root->parent = last_mid_node;
    }
    // 找到新加节点的头， 注意到root 一定是root->parent的第一个孩子
    // 比如新加1个节点，first_mid_node 就正好等于last_mid_node
    first_mid_node = first_mid_node->son;
    assert(first_mid_node->data == best_priority);
    // 新加item的那个leaf
    TreeNode *original_parent = first_mid_node->parent,
             *original_sibling = root->sibling;
    /*
        cur_parent
        /         \
  mid_nodes         ...
       /            \
      root -> ... -> siblings
    */
    // 这时候root的sibling还指向原来的sibling,不过parent已经改掉了
    // root 是第一个孩子
    assert(original_parent->son == first_mid_node);

    original_parent->son = root->sibling;
    root->sibling = nullptr;
    // 新item对应的leaf，放在第一个son的位置
    TreeNode* new_node = new TreeNode(item, last_time_);
    last_mid_node->son = new_node;
    new_node->sibling = root;
    new_node->parent = last_mid_node;
    TreeNode *cur_parent = original_parent, *cur_insert;
    // 如果新子树最高priority更高，要把新的子树移到对的位置
    while (cur_parent->parent != nullptr && best_priority < cur_parent->data) {
        // bestprior不可能等于cur
        // data，要不前面那个NoAncestor的条件一定过不了
        assert(best_priority != cur_parent->data);
        /* 然后是，为什么一定是加在original_parent 的祖先节点下面呢？
         * 因为新节点不会和任何其他的非祖先节点(如果
         * 有，记为mid')有相同且比所有祖先节点高的优先级
         * 否则，原来的root节点应该出现在mid'的下面
         */
        //   原来有的节点不一定全要加上去，因为可能形成的新子树优先级高过了
        //   原来的这个节点，以至于item其实并不能进入它。
        if (!__SamePriority(cur_parent, item)) {
            cur_parent = cur_parent->parent;
            continue;
        }
        //   要从上到下遍历，看哪个地方的优先级是对的，加到那里。
        cur_insert = first_mid_node;
        while (cur_insert->son && cur_parent->data > cur_insert->data) {
            // 祖先链条中不会有相同data
            cur_insert = cur_insert->son;
        }
        assert(cur_parent->data != cur_insert->data);
        /**
         * cur_insert -> node1
         *                |
         *               node2 / leaf
         * 新节点应该加在node1上面
         */
        TreeNode* new_node = new TreeNode();
        new_node->item = item;
        new_node->add_time = last_time_;
        new_node->data = cur_parent->data;
        new_node->son = cur_insert;
        new_node->parent = cur_insert->parent;
        cur_insert->parent = new_node;
        // 这个新的支一定是第一个孩子
        new_node->parent->son = new_node;
        // ShowTreePretty(original_parent, "");
        cur_parent = cur_parent->parent;
    }
    // best_p > cur->data, first_mid_node 要加在cur的下面
    first_mid_node->parent = cur_parent;
    first_mid_node->sibling = cur_parent->son;
    cur_parent->son = first_mid_node;
    // 加完了
    // 需要删掉独支(一个或多个只有一个孩子的中间节点)
    __DelBadNodes(original_sibling);
    // 保证新增节点之后顺序是对的
    // 现在应该不用了，因为之前让新子树跑到上面的情况里，访问的时候不更新中间节点位置
    // // 更新中间节点指的item，让item是下面最新的一个
    // int add_time = root->add_time;
    // __UpdateMidNodes(add_time, original_sibling);
    // // 对original_sibling 的祖先中间节点按照->item的加入时间进行sort
    // __SortAncestors(original_sibling);
    return;
}

int CartTree::__SubTreePriority(TreeNode* root, Item* item, TreeNode* node0) {
    assert(root != nullptr);
    /*
        找到当前节点为根的子树中，和item相符合且和指定节点(node0)的祖先不一样的优先级中的最高优先级的discount
        遍历root下的所有叶子，返回最高的优先级.
    */
    // exit
    if (root->son == nullptr) {
        // 子树就是这个叶子，所以只要判断一个节点
        for (int j = 0; j < MAX_DISCOUNT_1; j++) {
            // 跨店折扣类型相等且有折扣
            if (item->discount_1[j] == root->item->discount_1[j] &&
                item->discount_1[j]) {
                if (__NoAncestor(j, node0)) return j;
            }
        }
        if (item->shop_id == root->item->shop_id) {
            // same shop
            if (__NoAncestor(MAX_DISCOUNT_1, node0)) return MAX_DISCOUNT_1;
        }
        for (int j = 0; j < MAX_DISCOUNT_2; j++) {
            // all possible 满减
            if (item->discount_2[j] == root->item->discount_2[j] &&
                item->discount_2[j]) {
                if (__NoAncestor(j + MAX_DISCOUNT_1 + 1, node0))
                    return j + MAX_DISCOUNT_1 + 1;
            }
        }
        // 一个不包含在祖先的、一样的discount/商店都没
        return MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    }
    // Not leaf, recursion

    int this_priority, best_priority = MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    int tmp_priority = root->data;
    // 要求item的cur->data优先级和cur->item 的一样才可以进这个节点
    // 如果root->data == -1说明是root_ (到这里说明一定不是叶子)

    TreeNode *best_son, *cur = root->son;
    while (cur != nullptr) {
        this_priority = __SubTreePriority(cur, item, node0);
        if (this_priority < best_priority) {
            best_priority = this_priority;
            best_son = cur;
        }
        cur = cur->sibling;
    }
    // 如果能进这个节点(root)，直接返回
    // 如果不能进这个节点(root)，只有比node对应的优先级更高的才能返回

    if (__SamePriority(root, item)) {
        return best_priority;
    } else if (best_priority < node0->data) {
        return best_priority;
    } else {
        return MAX_DISCOUNT_1 + MAX_DISCOUNT_2 + 1;
    }
}

bool CartTree::__SamePriority(TreeNode* node, Item* item) {
    int tmp_priority = node->data;
    if (tmp_priority < MAX_DISCOUNT_1 && tmp_priority >= 0 &&
        node->item->discount_1[tmp_priority] == item->discount_1[tmp_priority])
        return true;
    else if (tmp_priority == MAX_DISCOUNT_1 &&
             node->item->shop_id == item->shop_id)
        return true;
    else if (tmp_priority > MAX_DISCOUNT_1 &&
             (node->item->discount_2[tmp_priority - MAX_DISCOUNT_1 - 1] ==
              item->discount_2[tmp_priority - MAX_DISCOUNT_1 - 1]))
        return true;

    return false;
}

bool CartTree::__NoAncestor(int data, TreeNode* node) {
    assert(node->parent != nullptr);
    // 不应该包括即将访问的那个节点
    node = node->parent;
    while (node != root_) {
        if (node->data == data) return false;
        assert(node->parent != nullptr);
        node = node->parent;
    }
    return true;
}

void CartTree::__DelBadNodes(TreeNode* leaf) {
    TreeNode* cur = leaf;
    if (leaf && leaf->parent && !leaf->son)
        // 如果leaf是nullptr, 中间节点, 或根, 不删
        cur = leaf->parent;
    else
        return;
    // 注意到这里如果leaf上方有独支但是leaf有多个item，也不必保留这个支
    while (cur != root_) {
        if (!cur->son->sibling) {
            // 把值搬过去
            cur->item = leaf->item;
            cur->data = -1;
            cur->add_time = leaf->add_time;
            cur->number = leaf->number;
            cur->son->~TreeNode();
            cur->son = nullptr;
        } else
            break;
        cur = cur->parent;
    }
    return;
}

TreeNode* CartTree::__get_prev_sibling(TreeNode* node) {
    assert(node->parent != nullptr);
    TreeNode* cur = node->parent->son;
    while (cur != nullptr && cur->sibling != node) {
        cur = cur->sibling;
    }
    // if node is the first sibling, should return nullptr.
    return cur;
}

#endif