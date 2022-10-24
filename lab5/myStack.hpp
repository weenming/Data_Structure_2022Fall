#include <iostream>
using namespace std;

#define MAXSIZE 10000
template <typename T>
class Stack {
   private:
    int top;
    unsigned int maxSize;
    T *arr;

   public:
    Stack();
    ~Stack();

    void push(T data);  // 推入元素
    T pop();            // 弹出栈顶元素
    T get_top();        // 获取栈顶元素
    unsigned int count();
    bool is_empty();
    bool is_full();
};

template <typename T>
Stack<T>::Stack() {
    maxSize = MAXSIZE;
    arr = new T[maxSize];
    top = -1;
    return;
}

template <typename T>
Stack<T>::~Stack() {
    delete[] arr;
    return;
}

template <typename T>
void Stack<T>::push(T data) {
    if (is_full()) {
        printf("is full!");
    }
    arr[++top] = data;
    return;
}

template <typename T>
T Stack<T>::pop() {
    if (is_empty()) {
        printf("is empty!");
    }
    return arr[top--];
}

template <typename T>
T Stack<T>::get_top() {
    return arr[top];
}

template <typename T>
unsigned int Stack<T>::count() {
    return top + 1;
}

template <typename T>
bool Stack<T>::is_empty() {
    if (top == -1) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
bool Stack<T>::is_full() {
    if (top == maxSize - 1) {
        return true;
    } else {
        return false;
    }
}
