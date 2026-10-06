// Task 1: Browser Tab Manager using a Circular Doubly Linked List
#include <iostream>
#include <string>
#include <limits>
using namespace std;
struct Tab {
    int id;
    string title, url;
    Tab *next, *prev;
    Tab(int i, const string& t, const string& u) : id(i), title(t), url(u), next(this), prev(this) {}
};
class TabManager {
    Tab* current = nullptr;  
public:
    ~TabManager() { while (current) closeCurrent(false); }

    void openTab(int id, const string& title, const string& url) {
        Tab* n = new Tab(id, title, url);
        if (!current) { current = n; return; }      
        n->next = current->next;
        n->prev = current;
        current->next->prev = n;
        current->next = n;
        
    }

    
    void closeCurrent(bool verbose = true) {
        if (!current) { if (verbose) cout << "No tabs open.\n"; return; }
        Tab* del = current;
        if (del->next == del) current = nullptr;     // only tab
        else {
            del->prev->next = del->next;
            del->next->prev = del->prev;
            current = del->next;
        }
        if (verbose) cout << "Closed tab " << del->id << ".\n";
        delete del;
    }

    void moveNext() { if (current) current = current->next; else cout << "No tabs open.\n"; }
    void movePrev() { if (current) current = current->prev; else cout << "No tabs open.\n"; }

    static void show(const Tab* t) {
        cout << "ID: " << t->id << " | Title: " << t->title << " | URL: " << t->url << "\n";
    }

    void displayCurrent() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        show(current);
    }

    void displayForward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* t = current;
        do { show(t); t = t->next; } while (t != current);
    }

    void displayBackward() const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* t = current;
        do { show(t); t = t->prev; } while (t != current);
    }

    void search(int id) const {
        if (!current) { cout << "No tabs open.\n"; return; }
        Tab* t = current;
        do {
            if (t->id == id) { cout << "Found -> "; show(t); return; }
            t = t->next;
        } while (t != current);
        cout << "Tab with ID " << id << " not found.\n";
    }
};

int main() {
    TabManager tm;
    int choice;
    do {
        cout << "\n--- Browser Tab Manager ---\n"
                "1. Open New Tab\n2. Close Current Tab\n3. Move Next\n4. Move Previous\n"
                "5. Display Current Tab\n6. Display All Forward\n7. Display All Backward\n"
                "8. Search Tab by ID\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        switch (choice) {
            case 1: {
                int id; string title, url;
                cout << "Tab ID: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Title: "; getline(cin, title);
                cout << "URL: "; getline(cin, url);
                tm.openTab(id, title, url);
                break;
            }
            case 2: tm.closeCurrent(); break;
            case 3: tm.moveNext(); break;
            case 4: tm.movePrev(); break;
            case 5: tm.displayCurrent(); break;
            case 6: tm.displayForward(); break;
            case 7: tm.displayBackward(); break;
            case 8: { int id; cout << "Tab ID to search: "; cin >> id; tm.search(id); break; }
            case 0: cout << "Bye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}