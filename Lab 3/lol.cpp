#include <iostream>
#include <string>
using namespace std;

class StringPool {
public:
    string* stringPool;   // dynamic array of strings
    int currentSize;      // current number of strings in the pool
    int maxSize;           // maximum size of the pool
    bool* isRemoved;       // keeps track of which slots were removed

    // constructor - initializes the fields
    StringPool() {
        maxSize = 5;
        currentSize = 0;
        stringPool = new string[maxSize];
        isRemoved = new bool[maxSize];
        for (int i = 0; i < maxSize; i++) {
            isRemoved[i] = false;
        }
    }

    // adds a string to the pool
    void addString(string value) {
        if (currentSize < maxSize) {
            stringPool[currentSize] = value;
            isRemoved[currentSize] = false;
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
            isRemoved[index] = true;
            // notice: stringPool[index] still holds the old data
            // this is the memory leak we fix later
        } else {
            cout << "Invalid index." << endl;
        }
    }

    // detects and fixes the leak by clearing removed strings
    void fixLeaks() {
        int fixedCount = 0;
        for (int i = 0; i < currentSize; i++) {
            if (isRemoved[i] && !stringPool[i].empty()) {
                cout << "Fixing leak at index " << i << " (was: " << stringPool[i] << ")" << endl;
                stringPool[i] = ""; // properly clear the data now
                fixedCount++;
            }
        }
        cout << "Total leaks fixed: " << fixedCount << endl;
    }

    // shows the current status of the pool
    void displayStatus() {
        cout << "\n--- Pool Status ---" << endl;
        cout << "currentSize: " << currentSize << " / maxSize: " << maxSize << endl;
        for (int i = 0; i < currentSize; i++) {
            cout << i << ": ";
            if (isRemoved[i]) {
                if (stringPool[i].empty()) {
                    cout << "(removed, memory freed)";
                } else {
                    cout << "(removed, but LEAK -> " << stringPool[i] << ")";
                }
            } else {
                cout << stringPool[i];
            }
            cout << endl;
        }
        cout << "-------------------\n" << endl;
    }

    // destructor - frees the dynamic arrays
    ~StringPool() {
        delete[] stringPool;
        delete[] isRemoved;
    }
};

int main() {
    StringPool pool;

    cout << "Pool created." << endl;
    cout << "maxSize: " << pool.maxSize << endl;
    cout << "currentSize: " << pool.currentSize << endl;
    cout << endl;

    // 1. add multiple strings to the pool
    pool.addString("apple");
    pool.addString("banana");
    pool.addString("cherry");
    pool.addString("date");
    pool.addString("elderberry");

    pool.displayStatus();

    // 2. remove strings without freeing memory
    pool.removeString(1); // banana
    pool.removeString(3); // date

    pool.displayStatus();

    // 3. detect and fix the memory leak, then display pool status
    cout << "Detecting and fixing memory leaks..." << endl;
    pool.fixLeaks();

    pool.displayStatus();

    return 0;
}