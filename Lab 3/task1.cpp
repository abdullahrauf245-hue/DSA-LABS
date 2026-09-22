#include <iostream>
#include <string>
using namespace std;

int main() {
    string s;
    cout << "Enter a string: ";
    getline(cin, s);

    // clean the string: keep only letters and digits, make lowercase
    string clean = "";
    for (int i = 0; i < s.length(); i++) {
        char ch = s[i];
        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9')) {
            clean = clean + ch;
        } else if (ch >= 'A' && ch <= 'Z') {
            ch = ch + 32; // convert to lowercase
            clean = clean + ch;
        }
    }

    // check palindrome using two pointers
    int left = 0;
    int right = clean.length() - 1;
    bool isPalindrome = true;

    while (left < right) {
        if (clean[left] != clean[right]) {
            isPalindrome = false;
            break;
        }
        left++;
        right--;
    }

    if (isPalindrome) {
        cout << s << " is a palindrome." << endl;
    } else {
        cout << s << " is NOT a palindrome." << endl;
    }

    return 0;
}