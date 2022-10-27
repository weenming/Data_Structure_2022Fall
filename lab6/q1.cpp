#include <iostream>

using namespace std;
#define MAXPOS 1.79769313486231570E+308
#define MINNEG -1.79769313486231570E+308
#define MINPOS 4.94065645841246544E-324
#define MAXNEG -4.94065645841246544E-324

double myPow(double x, int n) {
    // Time complexity: O(N)
    // Space complexity: O(N)
    // when n is zero, $x^n \def 0$
    double f;
    if (n == 0) {
        if (x == 0) {
            double myInf = 1e300 * 1e300;
            cout << "0 ^ 0 is undefined!" << endl;
            return myInf * 0;
        }
        return 1;
    }
    // 0 ^ n = 0
    if (x == 0) {
        return 1;
    }

    // exits of recursion:
    if (n == 1) {
        // when recursion hits n == 1, terminate
        // this applies to the case where initial n \ge 1
        return x;
    } else if (n == -1) {
        // when recursion hits n == -1, terminate
        // this applies to the case where initial n \le 1
        return 1 / x;
    }

    // recursion
    if (n > 1) {
        // when initial n > 1
        // when overflow, throw error
        f = x * myPow(x, n - 1);
        // detect overflow: I think my compiler optimizes this overflow to inf
        // if I do not declare `double myInf` explicitly, so this may not
        // function properly in other environments.
        double myInf = 1e300 * 1e300;
        if (f == myInf || f == -myInf) {
            cout << "overflow!" << endl;
            return f;
        }
        return f;
    }
    if (n < -1) {
        // when initial n < -1
        // when overflow, throw error
        f = myPow(x, n + 1) / x;
        // underflow: if initial x were zero, could not have reached here
        if (f == 0) {
            cout << "underflow!" << endl;
            return f;
        }
        return f;
    }
}

void testMyPow() {
    int n;
    double x;
    // test case 1
    cout << myPow(5, 1) << "\n" << endl;
    // test case 2
    cout << myPow(5, 3) << "\n" << endl;
    // test case 3
    cout << myPow(5, -1) << "\n" << endl;
    // test case 4
    cout << myPow(5, -3) << "\n" << endl;
    // test case 5
    cout << myPow(0.5, 3) << "\n" << endl;
    // test case 6
    cout << myPow(0.5, -3) << "\n" << endl;
    // test case 7
    cout << myPow(0, 0) << "\n" << endl;
    // test case 8
    cout << myPow(0, 1000) << "\n" << endl;
    // test case 9
    cout << myPow(1000, 0) << "\n" << endl;

    // test case 10: positive overflow
    // double should be 64 bits
    cout << myPow(10, 308) << "\n" << endl;
    // test case 11: positive overflow
    cout << myPow(10, 309) << "\n" << endl;

    // test case 12: negative overflow
    // double should be 64 bits
    cout << myPow(-10, 308) << "\n" << endl;
    // test case 13: negative overflow
    cout << myPow(-10, 309) << "\n" << endl;

    // test case 14: positive underflow (imprecise because approaching lower
    // bound of double digits)
    // double should be 64 bits
    cout << myPow(10, -323) << "\n" << endl;
    // test case 15: positive underflow
    cout << myPow(10, -324) << "\n" << endl;

    // test case 16: negative underflow
    // double should be 64 bits
    cout << myPow(-10, -323) << "\n" << endl;
    // test case 17: negative underflow
    cout << myPow(-10, -324) << "\n" << endl;

    return;
}

int main() {
    testMyPow();
    return 0;
}