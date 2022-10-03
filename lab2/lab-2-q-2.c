#include <stdio.h>
#include <stdlib.h>

/*
For Example
myPrint0 Expected output
00 01 02 03 04
10 11 12 13 14
20 21 22 23 24
30 31 32 33 34
40 41 42 43 44
*/
void myPrint0(int **arr, int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%02d ", arr[i][j]);
        }
        printf("\n");
    }
}

/*
For Example
myPrint1 Expected output
00
10 11
20 21 22
30 31 32 33
40 41 42 43 44
*/
void myPrint1(int **arr, int n) {
    // j: column; i: row
    // print all j <= i and move to next row
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > i) {
                break;
            }
            printf("%02d ", arr[i][j]);
        }
        printf("\n");
    }
}

/*
For Example
myPrint2 Expected output
00 01 02 03 04
11 12 13 14
22 23 24
33 34
44
*/
void myPrint2(int **arr, int n) {
    // iterate the whole array. print if column > row (the upper right matrix)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j >= i) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint3 Expected output
00 01 02 03 04
10 11 12 13
20 21 22
30 31
40
*/
void myPrint3(int **arr, int n) {
    // iterate the whole array. print if column < n - row + 1 (the upper left
    // matrix)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j < n - i) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint4 Expected output
04
13 14
22 23 24
31 32 33 34
40 41 42 43 44
*/
void myPrint4(int **arr, int n) {
    // iterate the whole array. print if column > n - row + 1 (the lower right
    // matrix)
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j >= n - i - 1) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint5 Expected output
00 01
10 11 12
21 22 23
32 33 34
43 44
*/
void myPrint5(int **arr, int n) {
    // iterate the whole array. print if column in [row - 1, row + 1], elements
    // in a band matrix with width 3
    int a = 2;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > i - a && j < i + a) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint6 Expected output
03 04
12 13 14
21 22 23
30 31 32
40 41
*/
void myPrint6(int **arr, int n) {
    // iterate the whole array. print if column in [n - row - 1, n - row + 1],
    // elements in a transposed band matrix with width 3
    int a = 2;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j < n - 1 - i + a && j > n - 1 - i - a) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint7 Expected output
04
13
22
31
40
*/
void myPrint7(int **arr, int n) {
    // iterate the whole array. print if column = n - row, diagonal elements in
    // the transposed matrix
    int a = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j < n - 1 - i + a && j > n - 1 - i - a) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

/*
For Example
myPrint8 Expected output
00
11
22
33
44
*/
void myPrint8(int **arr, int n) {
    // iterate the whole array. print if column = row, diagonal elements in the
    // transposed matrix
    int a = 1;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (j > i - a && j < i + a) {
                printf("%02d ", arr[i][j]);
            }
        }
        printf("\n");
    }
}

void test1() {
    // const int n = 5;
    const int n = 1;
    // const int n = 2;
    // const int n = 10;

    int **arr = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i) {
        arr[i] = (int *)malloc(n * sizeof(int));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            arr[i][j] = i * 10 + j;
        }
    }
    myPrint0(arr, n);
    printf("\n");
    myPrint1(arr, n);
    printf("\n");
    myPrint2(arr, n);
    printf("\n");
    myPrint3(arr, n);
    printf("\n");
    myPrint4(arr, n);
    printf("\n");
    myPrint5(arr, n);
    printf("\n");
    myPrint6(arr, n);
    printf("\n");
    myPrint7(arr, n);
    printf("\n");
    myPrint8(arr, n);
    printf("\n");

    myPrint0(arr, n);
    printf("\n");
    return;
}

int main() {
    test1();
    return 0;
}