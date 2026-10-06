
#include <iostream>
#include <string>
#include <limits>
using namespace std;

struct Coach {
    int number;
    string type;
    int capacity, passengers;
    Coach *next, *prev;
    Coach(int n, const string& t, int c, int p)
        : number(n), type(t), capacity(c), passengers(p), next(this), prev(this) {}
};
class Train {
    Coach* head = nullptr;      
    Coach* current = nullptr;   
    void removeNode(Coach* c) {
        if (c->next == c) { head = current = nullptr; }
        else {
            c->prev->next = c->next;
            c->next->prev = c->prev;
            if (head == c) head = c->next;
            if (current == c) current = c->next;   // next valid coach becomes current
        }
        delete c;}
public:
    ~Train() { while (head) removeNode(head); }
    static void show(const Coach* c) {
        cout << "Coach #" << c->number << " | Type: " << c->type
             << " | Capacity: " << c->capacity << " | Passengers: " << c->passengers
             << " | Empty seats: " << c->capacity - c->passengers << "\n";
    }
    Coach* find(int num) const {
        if (!head) return nullptr;
        Coach* t = head;
        do { if (t->number == num) return t; t = t->next; } while (t != head);
        return nullptr;}
    void addCoach(int num, const string& type, int cap, int pass) {
        if (find(num)) { cout << "Coach " << num << " already exists.\n"; return; }
        Coach* c = new Coach(num, type, cap, pass);
        if (!head) { head = current = c; return; }
        Coach* tail = head->prev;
        c->next = head;
        c->prev = tail;
        tail->next = c;
        head->prev = c;}
    void insertCoach(int after, int num, const string& type, int cap, int pass) {
        Coach* p = find(after);
        if (!p) { cout << "Coach " << after << " not found.\n"; return; }
        if (find(num)) { cout << "Coach " << num << " already exists.\n"; return; }
        Coach* c = new Coach(num, type, cap, pass);
        c->next = p->next;
        c->prev = p;
        p->next->prev = c;
        p->next = c;}
    void removeCoach(int num) {
        Coach* c = find(num);
        if (!c) { cout << "Coach " << num << " not found.\n"; return; }
        removeNode(c);
        cout << "Coach " << num << " removed.\n";}
    void moveForward()  { if (current) current = current->next; else cout << "Train is empty.\n"; }
    void moveBackward() { if (current) current = current->prev; else cout << "Train is empty.\n"; }
    void displayClockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* t = head;
        do { show(t); t = t->next; } while (t != head);}
    void displayAnticlockwise() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* t = head->prev;          
        do { show(t); t = t->prev; } while (t != head->prev);}
    void search(int num) const {
        Coach* c = find(num);
        if (c) { cout << "Found -> "; show(c); }
        else cout << "Coach " << num << " not found.\n";
    }
    void maxAvailable() const {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* best = head;
        Coach* t = head->next;
        while (t != head) {
            if (t->capacity - t->passengers > best->capacity - best->passengers) best = t;
            t = t->next;
        }
        cout << "Most available capacity -> "; show(best);}
    void displayCurrent() const {
        if (!current) { cout << "Train is empty.\n"; return; }
        show(current);}
    void reverseTrain() {
        if (!head) { cout << "Train is empty.\n"; return; }
        Coach* t = head;
        do {
            Coach* nxt = t->next;       
            t->next = t->prev;
            t->prev = nxt;
            t = nxt;                    
        } while (t != head);
        head = head->next;              
        cout << "Train direction reversed.\n";
    }
};
static void readCoach(int& n, string& t, int& c, int& p) {
    cout << "Coach Number: "; cin >> n; cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Coach Type: "; getline(cin, t);
    do {
        cout << "Passenger Capacity: "; cin >> c;
        cout << "Current Passengers: "; cin >> p;
       if (p < 0 || p > c) cout << "Passengers must be between 0 and capacity. Try again.\n";
    } while (p < 0 || p > c);}
int main() {
    Train tr;
    int n;
    cout << "Number of coaches: "; cin >> n;
    for (int i = 0; i < n; i++) {
        int num, cap, pass; string type;
        cout << "-- Coach " << i + 1 << " --\n";
        readCoach(num, type, cap, pass);
        tr.addCoach(num, type, cap, pass);
    }
    int choice;
    do {
        cout << "\n--- Train Coach System ---\n"
                "1. Add Coach\n2. Insert Coach After\n3. Remove Coach\n4. Move Forward\n"
                "5. Move Backward\n6. Display Clockwise\n7. Display Anti-clockwise\n"
                "8. Search Coach\n9. Max Available Capacity\n10. Display Current Coach\n"
                "11. Reverse Train Direction\n0. Exit\nChoice: ";
        if (!(cin >> choice)) break;
        int num, cap, pass, after; string type;
        switch (choice) {
            case 1: readCoach(num, type, cap, pass); tr.addCoach(num, type, cap, pass); break;
            case 2: cout << "Insert after coach #: "; cin >> after;
                    readCoach(num, type, cap, pass); tr.insertCoach(after, num, type, cap, pass); break;
            case 3: cout << "Coach # to remove: "; cin >> num; tr.removeCoach(num); break;
            case 4: tr.moveForward(); break;
            case 5: tr.moveBackward(); break;
            case 6: tr.displayClockwise(); break;
            case 7: tr.displayAnticlockwise(); break;
            case 8: cout << "Coach # to search: "; cin >> num; tr.search(num); break;
            case 9: tr.maxAvailable(); break;
            case 10: tr.displayCurrent(); break;
            case 11: tr.reverseTrain(); break;
            case 0: cout << "Bye!\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
    return 0;
}