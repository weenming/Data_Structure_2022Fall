#ifndef MYHEAP_HPP
#define MYHEAP_HPP

#define MAXSIZE 1000
#include <vector>
using namespace std;
int pow(int x, int n) {
    if (n == 0) {
        return 1;
    }
    int res = x;
    while (n > 1) {
        res = res * x;
        n--;
    }
    return res;
}

template <typename T>
class MyHeap {
    /*
    最小堆
    */
   private:
    vector<T> heap_;
    int current_size_;
    void __SiftUp();
    void __SiftDown();

   public:
    MyHeap();
    // initialize with array
    MyHeap(T*, int);
    // initialize with vector
    MyHeap(vector<T>);
    void Add(T);
    T PopTop();
    void ShowHeap();
    bool IsEmpty();
};

template <typename T>
MyHeap<T>::MyHeap() {
    heap_ = vector<T>(MAXSIZE);
    current_size_ = 0;
}

template <typename T>
MyHeap<T>::MyHeap(T* arr, int len) {
    if (len > MAXSIZE) {
        cout << "Error: full!" << endl;
    }

    heap_ = new T[MAXSIZE];
    current_size_ = 0;
    for (int i = 0; i < len; i++) {
        Add(arr[i]);
    }
    // void 函数应该写return吗?
    return;
}

template <typename T>
MyHeap<T>::MyHeap(vector<T> vec) {
    if (vec.size() > MAXSIZE) {
        cout << "Error: full!" << endl;
    }

    heap_ = vec;
    current_size_ = 0;
    for (int i = 0; i < vec.size(); i++) {
        Add(vec[i]);
    }
    // void 函数应该写return吗?
    return;
}

template <typename T>
void MyHeap<T>::Add(T new_elem) {
    if (current_size_ >= MAXSIZE - 1) {
        cout << "Error: full!" << endl;
    }
    heap_[current_size_] = new_elem;
    current_size_++;
    __SiftUp();
    return;
}

template <typename T>
T MyHeap<T>::PopTop() {
    if (current_size_ <= 0) {
        cout << "Error: empty!" << endl;
    }
    T top = heap_[0];
    heap_[0] = heap_[current_size_ - 1];
    current_size_--;
    __SiftDown();
    return top;
}

template <typename T>
void MyHeap<T>::__SiftDown() {
    // 总是把顶的那个sift down
    // index i 的子节点是2i + 1 and 2i + 2
    int i = 0, j;
    T tmp;
    while (2 * i + 1 < current_size_) {
        // 有子节点
        j = 2 * i + 1;
        if (heap_[i] <= heap_[j] && heap_[i] <= heap_[j + 1]) {
            break;
        } else if (heap_[j] > heap_[j + 1]) {
            // swap cur with smaller son, i.e. right son
            tmp = heap_[i];
            heap_[i] = heap_[j + 1];
            heap_[j + 1] = tmp;
        } else {
            // swap cur with right son
            tmp = heap_[i];
            heap_[i] = heap_[j];
            heap_[j] = tmp;
        }
        i = j;
    }
    return;
}

template <typename T>
void MyHeap<T>::__SiftUp() {
    // 总把最后一个sift up
    int i = current_size_ - 1, j;
    T tmp;
    while (i > 0) {
        // 整除
        j = (i - 1) / 2;
        if (heap_[j] <= heap_[i]) {
            break;
        } else {
            tmp = heap_[i];
            heap_[i] = heap_[j];
            heap_[j] = tmp;
        }
        i = j;
    }
    return;
}

template <typename T>
void MyHeap<T>::ShowHeap() {
    int i = 0, layer = 0;
    while (i < current_size_) {
        cout << heap_[i] << ' ';
        if (i == pow(2, layer + 1) - 2) {
            layer++;
            cout << endl;
        }
        i++;
    }
    cout << endl << "end" << endl;
}

template <typename T>
bool MyHeap<T>::IsEmpty() {
    if (current_size_ == 0) {
        return true;
    } else {
        return false;
    }
}
#endif  // MYHEAP_HPP