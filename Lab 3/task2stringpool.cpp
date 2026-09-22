#include <iostream>
#include <string>
using namespace std;

class StringPool {
public:
    string* stringPool;   // dynamic array of strings
    int currentSize;      // current number of strings in the pool
    int maxSize;           // maximum size of the pool

    // constructor - initializes the fields
    StringPool() {
        maxSize = 5;
        currentSize = 0;
        stringPool = new string[maxSize];
    }
};

int main() {
    StringPool pool;

    cout << "Pool created." << endl;
    cout << "maxSize: " << pool.maxSize << endl;
    cout << "currentSize: " << pool.currentSize << endl;

    return 0;
}