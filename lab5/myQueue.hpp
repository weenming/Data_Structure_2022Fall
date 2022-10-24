#include <iostream>
using namespace std;

#define MAXSIZE 4

template <typename T>
class Queue {
   private:
    T *arr;
    int front;
    int rear;
    int count;
    unsigned int maxSize;

   public:
    Queue();
    ~Queue();
    void push_front(T data);  // 入队
    void push_rear(T data);
    T pop_front();  // 出队
    T pop_rear();

    T get_front();  // 取队头值
    T get_rear();   // 取出尾值

    bool is_empty();  // 判空
    bool is_full();   // 判满
};

template <typename T>
Queue<T>::Queue() {
    maxSize = MAXSIZE;
    front = maxSize >> 1 + 1;
    rear = maxSize >> 1;
    arr = new T[maxSize];
    return;
}

template <typename T>
Queue<T>::~Queue() {
    delete[] arr;
    return;
}

template <typename T>
void Queue<T>::push_front(T data) {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 2) {
        cout << "pushing to full queue!" << endl;
    }
    arr[(--front + maxSize) % maxSize] = data;
    return;
}

template <typename T>
void Queue<T>::push_rear(T data) {
    if ((rear - maxSize) % maxSize == (front + maxSize) % maxSize + 2) {
        cout << "pushing to full queue!" << endl;
    }
    arr[(++rear - maxSize) % maxSize] = data;
    return;
}

template <typename T>
T Queue<T>::pop_front() {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 1) {
        cout << "popping empty queue!" << endl;
    }
    return arr[(front++ + maxSize) % maxSize];
}

template <typename T>
T Queue<T>::pop_rear() {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 1) {
        cout << "popping empty queue!" << endl;
    }
    return arr[(rear-- - maxSize) % maxSize];
}

template <typename T>
T Queue<T>::get_front() {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 1) {
        cout << "getting element from empty queue 1!" << endl;
    }
    return arr[(front + maxSize) % maxSize];
}

template <typename T>
T Queue<T>::get_rear() {
    if ((front + maxSize) % maxSize == ((rear - maxSize) % maxSize) + 1) {
        cout << "getting element from empty queue 2!" << endl;
    }
    return arr[(rear - maxSize) % maxSize];
}

template <typename T>
bool Queue<T>::is_empty() {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 1) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool Queue<T>::is_full() {
    if ((front + maxSize) % maxSize == (rear - maxSize) % maxSize + 2) {
        return true;
    } else {
        return false;
    }
}