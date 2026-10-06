// Task 2: Circular Photo Album using a Circular Doubly Linked List
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Photo {
    int id;
    string name, date, location;
    Photo *next, *prev;
    Photo(int i, const string& n, const string& d, const string& l)
        : id(i), name(n), date(d), location(l), next(this), prev(this) {}
};

class Album {
    Photo* head = nullptr;      // first photo
    Photo* current = nullptr;   // currently selected photo
    int count = 0;

    // unlink and free a node, fixing head/current if needed
    void removeNode(Photo* p) {
        if (p->next == p) { head = current = nullptr; }
        else {
            p->prev->next = p->next;
            p->next->prev = p->prev;
            if (head == p) head = p->next;
            if (current == p) current = p->next;   // next photo becomes current
        }
        delete p;
        count--;
    }
public:
    ~Album() { while (head) removeNode(head); }

    static void show(const Photo* p) {
        cout << "ID: " << p->id << " | Name: " << p->name
             << " | Date: " << p->date << " | Location: " << p->location << "\n";
    }

    // Insert at end of the circular list
    void addPhoto(int id, const string& n, const string& d, const string& l) {
        Photo* p = new Photo(id, n, d, l);
        if (!head) { head = current = p; }
        else {
            Photo* tail = head->prev;
            p->next = head;
            p->prev = tail;
            tail->next = p;
            head->prev = p;
        }
        count++;
    }

    // Insert right after the current photo
    void insertAfterCurrent(int id, const string& n, const string& d, const string& l) {
        if (!current) { addPhoto(id, n, d, l); return; }
        Photo* p = new Photo(id, n, d, l);
        p->next = current->next;
        p->prev = current;
        current->next->prev = p;
        current->next = p;
        count++;
    }

    Photo* find(int id) const {
        if (!head) return nullptr;
        Photo* t = head;
        do { if (t->id == id) return t; t = t->next; } while (t != head);
        return nullptr;
    }

    void removeById(int id) {
        Photo* p = find(id);
        if (!p) { cout << "Photo " << id << " not found.\n"; return; }
        removeNode(p);
        cout << "Photo " << id << " removed.\n";
    }

    void removeCurrent() {
        if (!current) { cout << "Album is empty.\n"; return; }
        removeNode(current);
        cout << "Current photo removed.\n";
    }

    void moveNext() { if (current) current = current->next; else cout << "Album is empty.\n"; }
    void movePrev() { if (current) current = current->prev; else cout << "Album is empty.\n"; }

    void displayForward() const {
        if (!current) { cout << "Album is empty.\n"; return; }
        Photo* t = current;
        do { show(t); t = t->next; } while (t != current);
    }
    void displayBackward() const {
        if (!current) { cout << "Album is empty.\n"; return; }
        Photo* t = current;
        do { show(t); t = t->prev; } while (t != current);
    }

    void search(int id) const {
        Photo* p = find(id);
        if (p) { cout << "Found -> "; show(p); }
        else cout << "Photo " << id << " not found.\n";
    }

    void displayCount() const { cout << "Total photos: " << count << "\n"; }
};

static void readPhoto(int& id, string& n, string& d, string& l) {
    cout << "Photo ID: "; cin >> id; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Name: "; getline(cin, n);
    cout << "Date Taken: "; getline(cin, d);
    cout << "Location: "; getline(cin, l);
}

int main() {
    Album a;
    int n;
    cout << "Number of photos: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int id; string nm, d, l;
        cout << "-- Photo " << i + 1 << " --\n";
        readPhoto(id, nm, d, l);
        a.addPhoto(id, nm, d, l);
    }

    int choice;
    do {
        cout << "\n--- Photo Album ---\n"
                "1. Add Photo (end)\n2. Insert After Current\n3. Remove Photo by ID\n"
                "4. Remove Current Photo\n5. Move Next\n6. Move Previous\n"
                "7. Display Forward\n8. Display Backward\n9. Search Photo\n"
                "10. Count Photos\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        int id; string nm, d, l;
        switch (choice) {
            case 1: readPhoto(id, nm, d, l); a.addPhoto(id, nm, d, l); break;
            case 2: readPhoto(id, nm, d, l); a.insertAfterCurrent(id, nm, d, l); break;
            case 3: cout << "ID to remove: "; cin >> id; a.removeById(id); break;
            case 4: a.removeCurrent(); break;
            case 5: a.moveNext(); break;
            case 6: a.movePrev(); break;
            case 7: a.displayForward(); break;
            case 8: a.displayBackward(); break;
            case 9: cout << "ID to search: "; cin >> id; a.search(id); break;
            case 10: a.displayCount(); break;
            case 0: cout << "Bye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}