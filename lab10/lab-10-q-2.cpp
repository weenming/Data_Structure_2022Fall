#include <iostream>
#include <vector>

using namespace std;

struct Node {
    int key;
    int val;
    Node* next;
    Node(int x, int y, Node* node) : key(x), val(y), next(node) {}
    Node() {
        key = -1;
        val = -1;
        next = nullptr;
    }
};

class LinkedList {
   private:
    Node* head;

   public:
    LinkedList() { head = new Node(); }
    bool remove(int val);
    void add(int key, int val);
    Node* find(int key);
};

bool LinkedList::remove(int x) {
    Node* cur = head;
    while (cur->next) {
        if (cur->next->key == x) break;
        cur = cur->next;
    }
    if (!cur->next) return false;  // remove unsuccessful
    Node* tmp = cur->next;
    cur->next = tmp->next;
    delete tmp;
    return true;
}

void LinkedList::add(int x, int val) {
    Node* new_node = new Node(x, val, nullptr);
    new_node->next = head->next;
    head->next = new_node;
}

Node* LinkedList::find(int x) {
    Node* cur = head;
    while (cur->next) {
        if (cur->next->key == x) return cur->next;
        cur = cur->next;
    }
    return nullptr;
}

class MyHashMap {
   public:
    MyHashMap(int x)
        : len(x), lists(vector<LinkedList*>(x, new LinkedList())) {}

    void put(int key, int value) {
        LinkedList* list = lists[hash(key)];
        Node* found_node = list->find(key);
        if (!found_node)
            list->add(key, value);
        else
            found_node->val = value;
    }

    int get(int key) {
        LinkedList* list = lists[hash(key)];
        Node* found_node = list->find(key);
        if (found_node)
            return found_node->val;
        else
            return -1;
    }

    void remove(int key) { lists[hash(key)]->remove(key); }

   private:
    int len;
    vector<LinkedList*> lists;
    int hash(int x) { return x % len; }
};

void test1() {
    int key = 1;
    int value = 0xabcde;
    MyHashMap* obj = new MyHashMap(10);
    obj->put(key, value);
    cout << obj->get(key) << endl;
    obj->remove(key);
    cout << obj->get(key) << endl;
    return;
}

void test2() {
    // conflict
    int key1 = 1;
    int value1 = 0xabcde;

    int key2 = 11;
    int value2 = 0xbcdef;
    MyHashMap* obj = new MyHashMap(10);
    obj->put(key1, value1);
    obj->put(key2, value2);
    cout << "value 1" << obj->get(key1) << endl;
    cout << "value 2" << obj->get(key2) << endl;
    obj->remove(key1);
    cout << "value 1" << obj->get(key1) << endl;
    cout << "value 2" << obj->get(key2) << endl;
    obj->remove(key2);

    cout << "value 1" << obj->get(key1) << endl;
    cout << "value 2" << obj->get(key2) << endl;
    return;
}

void test3() {
    // different key-values
    int key1 = 1;
    int value1 = 0xabcde;

    int key2 = 21;
    int value2 = 0xbcdef;

    int key3 = 564;
    int value3 = 0xacdf;
    MyHashMap* obj = new MyHashMap(10);
    for (int i = 0; i < 100; i++) {
        obj->put(i, i * i * i);
    }
    for (int i = 0; i < 20; i += 3)
        cout << "value" << i << ':' << obj->get(i) << endl;
    return;
}

void test4() {
    // change mod
    int key1 = 1;
    int value1 = 0xabcde;

    int key2 = 21;
    int value2 = 0xbcdef;

    int key3 = 564;
    int value3 = 0xacdf;
    MyHashMap* obj = new MyHashMap(1);
    for (int i = 0; i < 100; i++) {
        obj->put(i, i * i * i);
    }
    for (int i = 0; i < 20; i += 3)
        cout << "value" << i << ':' << obj->get(i) << endl;

    obj = new MyHashMap(100);
    for (int i = 0; i < 100; i++) {
        obj->put(i, i * i * i);
    }
    for (int i = 0; i < 20; i += 3)
        cout << "value" << i << ':' << obj->get(i) << endl;
    return;
}

int main() {
    test4();
    return 0;
}
