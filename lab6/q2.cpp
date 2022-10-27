#include <iostream>

using namespace std;

/*solve using recursion: permutation of str_{from i to end} = {c_i + permutation
 * of str_{from i+1 to end}, C_i + permutation of str_{from i+1 to end}}*/

int permutation(char* s, int n) {
    /*
    Time complexity is: O(2 ^ n + m)
    Space complexity is: O(n + m): no need to copy the
    where n is the number of English letters in the given string and m is the
    number of digits
    */
    int count = 0;
    if (s[n] == 0) {
        int i = 0;
        while (s[i]) {
            cout << s[i++];
        }
        cout << endl;
        return 1;
    }
    char c = s[n];
    if (c >= 48 && c <= 57) {
        count += permutation(s, n + 1);
        return count;
    } else if (c >= 65 && c <= 90) {
        // print all permutations of the substring of s from the n-th char on
        // with the n-th char c in the uppercase
        count += permutation(s, n + 1);
        s[n] = s[n] + 32;
        // all permutations with c in the lowercase.
        count += permutation(s, n + 1);
        return count;
    } else if (c >= 97 && c <= 122) {
        count += permutation(s, n + 1);
        s[n] = s[n] - 32;
        count += permutation(s, n + 1);
        return count;
    } else {
        cout << "invalid character: " << c << " at " << n << endl;
        exit(1);
    }
    return count;
}

void test_permutation(void) {
    int const LEN = 100;
    // char s[LEN] = "a1b2c3";
    // char s[LEN] = "ABC123def";
    char s[LEN] = "123456";
    // char s[LEN] = "A";
    // Permutation count should be 2 ^ n, where n is the number of English
    // letters.
    cout << "total permutation number is " << permutation(s, 0);
    return;
}

int main() {
    test_permutation();
    return 0;
}