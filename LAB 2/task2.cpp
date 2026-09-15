#include <iostream>
using namespace std;

int main() {
    int a = 5, *pa;
    pa = &a;

    cout << "*pa = " << *pa << endl;
    cout << "*pa * 2 = " << *pa * 2 << endl;   // works: 10

    pa = pa * 2;   // ERROR: can't multiply the address itself
    cout << "pa * 2 = " << pa << endl;

    return 0;
}