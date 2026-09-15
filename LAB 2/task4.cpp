// TASK A

// #include <iostream>
// using namespace std;

// int main() {
//     int x[4] = {0, 4, 6, 9};
//     int *p, a = 3;
//     p = x;

//     (*p)++;
//     cout << *p << endl;
//     cout << *(p+1) << endl;
//     p++;
//     *p = *p + a;
//     cout << *p << endl;
//     p = p + 2;
//     cout << *p << endl;

//     return 0;
// }


#include <iostream>
using namespace std;

int main() {
    int a, *p, *q;
    int arr[4] = {0};
    p = arr;
    q = p;

    *p = 4;
    for (int i = 0; i < 3; i++) {
        a = *p;
        p++;
        *p = (a + i);
    }

    for (int j = 0; j < 4; j++) {
        cout << *q << " ";
        q++;
    }
    cout << endl;

    return 0;
}