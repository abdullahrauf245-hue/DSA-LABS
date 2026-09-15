#include <iostream>
using namespace std;

int main() {
    int list[5] = {3, 6, 9, 12, 15};
    int *pArr = list;

    for (int i = 0; i < 5; i++) {
        cout << *(pArr + i) << " ";   // pointer arithmetic, not array indexing
    }
    cout << endl;

    return 0;
}