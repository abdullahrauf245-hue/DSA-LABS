#include <iostream>
using namespace std;

int main() {
    int n;
    cout << "Enter n (1-10): ";
    cin >> n;

    int *marks = new int[n];
    cout << "Enter " << n << " marks: ";
    for (int i = 0; i < n; i++)
        cin >> *(marks + i);

    // allocate bigger block
    int *newMarks = new int[n + 1];
    for (int i = 0; i < n; i++)
        *(newMarks + i) = *(marks + i);   // copy

    cout << "Enter new mark: ";
    cin >> *(newMarks + n);

    delete[] marks;      // release old block
    marks = newMarks;    // point to new block
    n = n + 1;            // update size
    newMarks = nullptr;

    cout << "Updated marks: ";
    for (int i = 0; i < n; i++)
        cout << *(marks + i) << " ";
    cout << endl;

    delete[] marks;   // release final block, exactly once
    marks = nullptr;

    return 0;
}