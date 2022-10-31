// #include <string.h>
#include <iostream>
#include <vector>
using namespace std;

/*solve using recursion: permutation of str_{from i to end} = {c_i + permutation
 * of str_{from i+1 to end}, C_i + permutation of str_{from i+1 to end}}*/

vector<string> permutation(string s, int n) {
    /*
    Time complexity is: O(2 ^ n)
    Space complexity is: O(n * 2 ^ n): no need to copy the
    where n is the number of English letters in the given string and m is the
    number of digits
    */
    if (s[n] == 0) {
        vector<string> res;
        res.push_back(s);

        return res;
    }
    char c = s[n];
    if (c >= 48 && c <= 57) {
        return permutation(s, n + 1);
    } else if (c >= 65 && c <= 90) {
        // save all permutations of the substring of s from the n-th char on
        // with the n-th char c in the uppercase
        vector<string> origin(permutation(s, n + 1));
        s[n] = s[n] + 32;
        // all permutations with c in the lowercase.
        vector<string> changed(permutation(s, n + 1));
        origin.insert(origin.end(), changed.begin(), changed.end());
        return origin;

    } else if (c >= 97 && c <= 122) {
        // with the n-th char c in the uppercase
        vector<string> origin(permutation(s, n + 1));
        s[n] = s[n] - 32;
        // all permutations with c in the lowercase.
        vector<string> changed(permutation(s, n + 1));
        origin.insert(origin.end(), changed.begin(), changed.end());
        return origin;
    } else {
        cout << "invalid character: " << c << " at " << n << endl;
        exit(1);
    }
}

void test_permutation(void) {
    string s = "a1b2c3";
    // string s = "ABC123def";
    // string s = "123456";
    // string s = "zZ";
    // string s = "A";
    // There should be 2 ^ n permutations, where n is the number of English
    // letters.
    vector<string> str_vec(permutation(s, 0));
    // print returned vector
    for (auto it = str_vec.cbegin(); it != str_vec.cend(); it++) {
        std::cout << *it << ' ';
    }
    return;
}

int main() {
    // cout << "here" << endl;
    test_permutation();
    return 0;
}