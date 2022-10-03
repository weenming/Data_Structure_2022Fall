#include <iostream>
using namespace std;

int main(void) {
    int arr[100];
    for (int i = 0; i < 100; i++) {
        arr[i] = 200 - i;
    }
    int max = arr[0];
    int min = arr[0];
    for (int i = 1; i < 100; i++) {
        if (arr[i] > max) {
            max = arr[i];
        } else if (arr[i] < min) {
            min = arr[i];
        }
    }
    cout << "max is" << max;
    cout << "min is" << min;

    return 0;
}