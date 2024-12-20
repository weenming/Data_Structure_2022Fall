#ifndef TREE_H_
#define TREE_H_

#include <cassert>
#include <iomanip>
#include <iostream>
#include <string>

#include "globals.hpp"
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
    double __ShowCartRecur(TreeNode*);
    void __DelTree(TreeNode*);
    TreeNode* __AddItem(Item* item, bool update);
    TreeNode* __AddMidNode(TreeNode*, Item*);
    bool __NoAncestor(int, TreeNode*);
    TreeNode* __GetPrevSibling(TreeNode*);
    TreeNode* __FindLeaf(int shop_id, int item_id, TreeNode* root);
    TreeNode* __DelBadNodes(TreeNode*);
    TreeNode* __DelEmptyNodes(TreeNode*);
    void __UpdateMidNodesDel(int, TreeNode*);
    void __UpdateMidNodesAdd(TreeNode*);
    bool __SamePriority(TreeNode*, Item*);
    int last_time_;
    int item_count_;

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
        cout << "Total price: " << fixed << setprecision(2)
             << __ShowCartRecur(root_) << endl
             << endl;
    };
    void ShowTreePretty(TreeNode*, string);
};

void CartTree::ShowTreePretty(TreeNode* cur, string indent) {
    if (cur == nullptr) {
        return;
    }
    if (cur->data != -1) {
        cout << indent << "|- Node: data = " << cur->data;
        if (cur->data < MAX_DISCOUNT_1) {
            cout << " (跨店折扣" << cur->data + 1 << "-"
                 << char('a' - 1 + cur->item->discount_1[cur->data]) << ")"
                 << endl;
        } else if (cur->data == MAX_DISCOUNT_1) {
            cout << " (同商店" << cur->item->shop_id << ")" << endl;
        } else {
            cout << " (跨店满减" << cur->data - MAX_DISCOUNT_1 << "-"
                 << char('a' - 1 +
                         cur->item->discount_2[cur->data - MAX_DISCOUNT_1 - 1])
                 << ")" << endl;
        }
    } else if (cur->item != nullptr) {
        // leaf
        cout << indent << "|- Leaf-"
             << "-item number: " << cur->number
             << " add time: " << cur->add_time << endl;
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
    item_count_ = 0;
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

double CartTree::__ShowCartRecur(TreeNode* cur) {
    if (cur == nullptr) return 0;

    // cout << cur << endl;
    if (cur->son != nullptr) {
        return __ShowCartRecur(cur->son) + __ShowCartRecur(cur->sibling);
    } else if (cur != root_) {
        cur->item->ShowItemForCart();
        cout << "      - " << cur->number << " pcs in your shopping cart"
             << endl;
        cout << "      - "
             << "   add time: " << cur->add_time << endl;
        cout << "   price: " << fixed << setprecision(2) << cur->item->price
             << " * " << cur->number << endl
             << endl;
        return cur->item->price * cur->number + __ShowCartRecur(cur->sibling);
    } else {
        return 0;
    }
}

void CartTree::AddItem(Item* item) {
    cout << "Adding item: id " << item->item_id << " ..." << endl;
    if (item_count_ < MAX_ITEM_NUMBER) {
        // 更新加入时间和购物车内商品数量
        last_time_ += 1;
        item_count_ += 1;
        __AddItem(item, true);
        cout << "Add item success" << endl << endl;
        return;
    } else {
        cout << "Fail: shopping cart is full!" << endl << endl;
        return;
    }
}
TreeNode* CartTree::__AddItem(Item* item, bool update) {
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

    // find place to insert
    // 第一次添加
    if (root_->son == nullptr) {
        TreeNode* new_node = new TreeNode(item, last_time_);
        new_node->parent = root_;
        root_->son = new_node;
        return new_node;
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
            return new_node;
        }
        // 第二种情况：最好的是叶子
        else if (next_root->son == nullptr && update) {
            // 把叶子移到最前面
            cur = __GetPrevSibling(next_root);
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
        else if (__SamePriority(next_root, item) && update) {
            // 如果确实能访问这个节点，要更新
            // 由于新加的节点是最新的，所以把next root移到siblings中的最左边
            // 如果nextroot不是最左孩子，维护sibling指针
            cur = __GetPrevSibling(next_root);
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
        root = __AddMidNode(root, item);
    }
    return root;
}

TreeNode* CartTree::__AddMidNode(TreeNode* root, Item* item) {
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
    TreeNode* remain_node = __DelBadNodes(original_sibling);
    // 这个不太efficient，因为从头开始加了
    if (remain_node) {
        // 如果有删独支

        TreeNode* prev_node = __GetPrevSibling(remain_node);
        if (prev_node) {
            prev_node->sibling = remain_node->sibling;
        } else {
            remain_node->parent->son = remain_node->sibling;
        }
        // 再加回去，就按顺序了
        int time_now = last_time_;
        last_time_ = remain_node->add_time;
        TreeNode* tmp_node = __AddItem(remain_node->item, false);
        last_time_ = time_now;
        remain_node->~TreeNode();
        remain_node = tmp_node;
        __UpdateMidNodesAdd(remain_node);

        return tmp_node;
    } else {
        return new_node;
    }
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
                if (__NoAncestor(j, node0)) {
                    return j;
                }
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

    if (__SamePriority(node0, item)) {
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

TreeNode* CartTree::__DelBadNodes(TreeNode* leaf) {
    TreeNode *cur = leaf, *last_cur = leaf;
    if (leaf && leaf->parent && !leaf->son)
        // 如果leaf是nullptr, 中间节点, 或根, 不删
        cur = leaf->parent;
    else
        return nullptr;
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
            last_cur = cur;
        } else
            break;
        cur = cur->parent;
    }
    return last_cur;
}

TreeNode* CartTree::__GetPrevSibling(TreeNode* node) {
    assert(node && node->parent);
    TreeNode* cur = node->parent->son;
    while (cur != nullptr && cur->sibling != node) {
        cur = cur->sibling;
    }
    // if node is the first sibling, should return nullptr.
    return cur;
}

void CartTree::DelItem(int shop_id, int item_id) {
    /**
     * 删除商品
     * 输入：一个shop_id and item_id
     * 遍历树，找到这个商品
     *
     * (1) 找不到，打印消息
     *
     * (2) 找到相应商品大于1件, number-1 即可。不需要更新add_item
     *          因为京东也是这么干的...
     *          事实上京东在添加商品时，直接购物车里点数量+1是不会改变排序的，只有在商品详情
     *          页中添加到购物车才会正确排序...
     *
     * (3)   找到相应商品等于1件，删掉这个商品。
     *
     *    这个leaf一定有sibling
     *    a. 除非它直接接在root_下，这时直接删了就好了, 都不用重新排序了
     *    b. 如果这个leaf有1个sibling，要对这个sibling执行__DelBadNodes
     *       最后位置存为tmp_node
     *    c. 如果有2个以上sibling，这一步也不用额外操作
     *
     *    先更新改变的叶子的所有祖先的add_time:如果它等于删掉节点的add_time,要换成
     *    删掉的叶子的后一个sibling的add_time
     *    如果删的叶子没有sibling
     *
     *    最后排序，在每一层中只需要对删掉节点的祖先节点冒泡就行(向右侧sibling比较和移动，
     *    因为删掉了一个叶子，祖先节点的add_time一定是不变或者变旧的)
     *
     *
     */
    item_count_ -= 1;
    TreeNode* leaf_to_del = __FindLeaf(shop_id, item_id, root_);
    if (!leaf_to_del) {
        cout << "can not find the item to remove!" << endl;
        return;
    } else {
        cout << "deleting item: id " << item_id << endl;
    }
    // cout << leaf_to_del->item->item_id << endl;
    // 最简单：大于1件商品
    if (leaf_to_del->number > 1) {
        leaf_to_del->number -= 1;
        cout << "item " << item_id << " in shop " << shop_id << " deleted, "
             << leaf_to_del->number << "pcs of the same item remaining" << endl;
        return;
    }
    // 等于1件商品
    TreeNode *prev_node, *remain_node = nullptr, *tmp_node;
    // 先看有多少个sibling
    // 任意一个leaf都一定有sibling，不管是中间的节点还是leaf
    // 除非这个leaf是root_的孩子
    if (!leaf_to_del->parent->son->sibling) {
        // 唯一的孩子
        assert(leaf_to_del->parent == root_);
        leaf_to_del->~TreeNode();
        root_->son = nullptr;
        cout << "item " << item_id << " in shop " << shop_id
             << " deleted (last item in cart)" << endl;
        return;
    }

    assert(leaf_to_del->parent->son->sibling);
    // 如果有左边节点，那就不用sort了，but whatever
    prev_node = __GetPrevSibling(leaf_to_del);
    if (prev_node) {
        // remain_node是leaf_to_del之后最高优先级的那个节点
        remain_node = prev_node->parent->son;
        prev_node->sibling = leaf_to_del->sibling;
    } else {
        remain_node = leaf_to_del->sibling;
        leaf_to_del->parent->son = leaf_to_del->sibling;
    }
    // 如果就2个leaf，删掉1个还剩1个，要调整树
    if (!leaf_to_del->parent->son->sibling && !leaf_to_del->parent->son->son) {
        remain_node = __DelBadNodes(remain_node);
        // 这个不太efficient，因为从头开始加了
        // 从树中移除剩下的这个item
        prev_node = __GetPrevSibling(remain_node);
        if (prev_node) {
            prev_node->sibling = remain_node->sibling;
        } else {
            remain_node->parent->son = remain_node->sibling;
        }
        // 再加回去，就按顺序了
        int time_now = last_time_;
        last_time_ = remain_node->add_time;
        tmp_node = __AddItem(remain_node->item, false);
        last_time_ = time_now;
        remain_node->~TreeNode();
        remain_node = tmp_node;
    }
    // 不管怎样，删掉节点
    int deleted_add_time = leaf_to_del->add_time;
    leaf_to_del->~TreeNode();

    // 更新remain_node的祖先节点的add_time
    // 从remain_node开始，对祖先节点逐层排序
    __UpdateMidNodesDel(deleted_add_time, remain_node->parent->son);
    cout << "item " << item_id << " in shop " << shop_id << " deleted, "
         << endl;
    return;
}

TreeNode* CartTree::__FindLeaf(int shop_id, int item_id, TreeNode* root) {
    if (!root) return nullptr;
    if (!root->son) {
        if (root->item->shop_id == shop_id && root->item->item_id == item_id)
            return root;
        else
            return __FindLeaf(shop_id, item_id, root->sibling);
    } else {
        TreeNode* leaf_in_son = __FindLeaf(shop_id, item_id, root->son);
        if (leaf_in_son) return leaf_in_son;
        TreeNode* leaf_in_sibling = __FindLeaf(shop_id, item_id, root->sibling);
        if (leaf_in_sibling) return leaf_in_sibling;
    }
    return nullptr;
}

void CartTree::__UpdateMidNodesDel(int deleted_add_time,
                                   TreeNode* remain_node) {
    int remain_add_time = remain_node->add_time;
    TreeNode *cur = remain_node, *prev_sibling;
    while (cur && cur->parent) {
        if (cur->add_time == deleted_add_time ||
            cur->add_time < remain_add_time) {
            cur->add_time = remain_add_time;
        }
        // 向右边比较，如果addtime比右边小就和右边交换
        while (cur->sibling && cur->add_time < cur->sibling->add_time) {
            prev_sibling = __GetPrevSibling(cur);
            if (prev_sibling) {
                prev_sibling->sibling = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                prev_sibling->sibling->sibling = cur;
            } else {
                cur->parent->son = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                cur->parent->son->sibling = cur;
            }
        }
        cur = cur->parent;
    }
    return;
}

void CartTree::__UpdateMidNodesAdd(TreeNode* remain_node) {
    int remain_add_time = remain_node->add_time;
    TreeNode *cur = remain_node, *prev_sibling;
    while (cur && cur->parent) {
        if (cur->add_time < remain_add_time) {
            cur->add_time = remain_add_time;
        }
        // 向右边比较，如果addtime比右边小就和右边交换
        while (cur->sibling && cur->add_time < cur->sibling->add_time) {
            prev_sibling = __GetPrevSibling(cur);
            if (prev_sibling) {
                prev_sibling->sibling = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                prev_sibling->sibling->sibling = cur;
            } else {
                cur->parent->son = cur->sibling;
                cur->sibling = cur->sibling->sibling;
                cur->parent->son->sibling = cur;
            }
        }
        cur = cur->parent;
    }
    return;
}

#endif