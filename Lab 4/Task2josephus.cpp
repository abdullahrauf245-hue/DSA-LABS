#include <iostream>
using namespace std;
// Node of the circular linked list: one person
struct Person {
    int id;
    Person* next;
};
// Create Circle: build N people and link last to first
Person* createCircle(int n, Person*& tail) {
    Person* head = NULL;
    tail = NULL;
    for (int i = 1; i <= n; i++) {
        Person* p = new Person{i, NULL};
        if (!head) head = p;
        else tail->next = p;
        tail = p;
    }
    tail->next = head; // makes it circular
    return head;
}
int main() {
    int n, k;
    cout << "Enter number of people (N): "; cin >> n;
    cout << "Enter step count (k): "; cin >> k;
    if (n < 1 || k < 1) { cout << "Invalid input.\n"; return 0; }
    Person* tail;
    Person* curr = createCircle(n, tail);
    Person* prev = tail; // prev always stays one node behind curr
    int left = n;
    cout << "Elimination order: ";
    while (left > 1) {
        // move k-1 steps so curr is the k-th person
        for (int i = 1; i < k; i++) {
            prev = curr;
            curr = curr->next;
        }
        cout << curr->id << " ";
        prev->next = curr->next; // unlink curr
        Person* del = curr;
        curr = curr->next;
        delete del;
        left--;
    }
    cout << "\nSurvivor: " << curr->id << endl;
    delete curr; // free the last node
    return 0;
}