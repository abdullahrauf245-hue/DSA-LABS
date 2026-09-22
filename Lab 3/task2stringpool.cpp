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

    // adds a string to the pool
    void addString(string value) {
        if (currentSize < maxSize) {
            stringPool[currentSize] = value;
            currentSize++;
            cout << "Added: " << value << endl;
        } else {
            cout << "Pool is full, cannot add " << value << endl;
        }
    }

    // removes a string from the pool WITHOUT freeing memory
    void removeString(int index) {
        if (index >= 0 && index < currentSize) {
            cout << "Removed (without freeing): " << stringPool[index] << endl;
            // notice: nothing is cleared or deleted here on purpose
            // this leaves behind a memory leak that we deal with later
        } else {
            cout << "Invalid index." << endl;
        }
    }
};

int main() {
    StringPool pool;

    cout << "Pool created." << endl;
    cout << "maxSize: " << pool.maxSize << endl;
    cout << "currentSize: " << pool.currentSize << endl;

    cout << endl;
    pool.addString("apple");
    pool.addString("banana");
    pool.addString("cherry");

    cout << endl;
    pool.removeString(1);

    return 0;
}