#include <iostream>
#include <string>
using namespace std;

void preprocessing(string p, int* n) {
    int len = p.size();
    int k = 0;
    n[0] = -1;
    for (int i = 2; i < len; i++) {
        if (p[i - 1] == p[k]) {
            n[i] = k + 1;
            k++;
        } else if (k == 0) {
            n[i] = 0;
        } else {
            k = n[k];
            i--;
        }
    }
}

int find_substr(string str, string p) {
    /* Can not process empty string yet... */
    int next[p.size()];
    preprocessing(p, next);
    int i = 0;
    int j = 0;
    while (i < str.size()) {
        if (str[i] == p[j]) {
            i++;
            j++;
        } else if (next[j] == -1) {
            i++;
        } else {
            j = next[j];
        }
        if (j == p.size()) {
            return 1;
        }
        if (i == str.size()) {
            return 0;
        }
    }
}

int mySolution(string a, string b) {
    /*
        Time complexity: only need to do KMP once and therefore is O(a.size() +
       b.size()) Space complexity: c takes up max(a.size(), b.size()) space. KMP
       takes extra b.size(). So total space complexity is O(a.size(), b.size()).
    */
    // find the shortest repetitive string of a s.t. b is a substring of it
    int n = b.size() / a.size() + 1 - !(b.size() - a.size());
    string c = "";
    for (int i = 0; i < n; i++) {
        c = c + a;
    }
    // now a is repeated for n times
    if (find_substr(c, b)) {
        return n;
    } else if (find_substr(c + a, b)) {
        return n + 1;
    } else {
        return -1;
    }
}

void test() {
    printf("\nrepeated string test: \n");

    string s = "bca";
    string p = "abcabcabc";
    cout << mySolution(s, p) << " ";  // should be 4

    s = "bcabcab";
    p = "abcabcabc";
    cout << mySolution(s, p) << " ";  // should be -1

    s = "bcabcabca";
    p = "abcabcabc";
    cout << mySolution(s, p) << " ";  // should be 2

    s = "b";
    p = "abcabcabc";
    cout << mySolution(s, p) << " ";  // should be -1

    s = "badguy";
    p = "guy";
    cout << mySolution(s, p) << " ";  // should be 1

    s = "badguy";
    p = "badguy";
    cout << mySolution(s, p) << " ";  // should be 1

    s = "z";
    p = "z";
    cout << mySolution(s, p) << " ";  // should be 1

    cout << endl;
}

void test_KMP() {
    printf("\nKMP algo test: \n");
    string s = "abaabcac";
    string p = "abaabcacd";
    cout << find_substr(s, p) << " ";

    s = "abaabcac";
    p = "abaabcac";
    cout << find_substr(s, p) << " ";

    s = "abaabcac";
    p = "a";
    cout << find_substr(s, p) << " ";

    s = "abaabcac";
    p = "c";
    cout << find_substr(s, p) << " ";

    s = "abaabcac";
    p = "aab";
    cout << find_substr(s, p) << " ";

    s = "a";
    p = "a";
    cout << find_substr(s, p) << " ";

    cout << endl;
}

int main() {
    /* Only need to examine 2 cases:
    1: str1.size() >= str2.size()
        test  str1 and str1 + str1
    2: str1.size() < str2.size()
        suppose n = str2.size() / str1.size() + 1
        test  n * str1 and (n+1) * str1
    So they can be dealt with in the same way
    */
    test_KMP();
    test();
    return 0;
}
