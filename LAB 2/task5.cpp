#include <iostream>
using namespace std;
// Part (a): swap using single pointers
void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}
// Part (b): swap using double pointers (pointer-to-pointer)
void swapPtr(int **px, int **py) {
    int *temp = *px;
    *px = *py;
    *py = temp;
}
int main() {
    int a = 5, b = 10;
    int *pa = &a;
    int *pb = &b;
    int **ppa = &pa;
    int **ppb = &pb;
    cout << "Before swap(a,b): a = " << a << ", b = " << b << endl;
    swap(&a, &b);
    cout << "After swap(a,b): a = " << a << ", b = " << b << endl;
    cout << "\nBefore swapPtr: pa points to " << *pa << ", pb points to " << *pb << endl;
    swapPtr(ppa, ppb);
    cout << "After swapPtr: pa points to " << *pa << ", pb points to " << *pb << endl;
    return 0;
}