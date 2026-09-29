#include <iostream>
#include <string>
using namespace std;
// Node of the doubly linked list: one song
struct Song {
    int id;
    string name;
    int min, sec;
    Song* prev;
    Song* next;
};
class Playlist {
    Song* head;
    Song* tail;
    Song* current; // song that is currently playing
public:
    Playlist() { head = tail = current = NULL; }
    ~Playlist() {
        while (head) {
            Song* t = head;
            head = head->next;
            delete t;
        }
    }
    // 1. Add song at end
    void addSong(int id, string name, int m, int s) {
        Song* n = new Song{id, name, m, s, tail, NULL};
        if (tail) tail->next = n;
        else head = n;
        tail = n;
        cout << "Song added.\n";
    }
    Song* find(int id) {
        for (Song* t = head; t; t = t->next)
            if (t->id == id) return t;
        return NULL;
    }
    // 2. Delete by id
    void deleteSong(int id) {
        Song* t = find(id);
        if (!t) { cout << "Song not found.\n"; return; }
        if (t->prev) t->prev->next = t->next;
        else head = t->next;
        if (t->next) t->next->prev = t->prev;
        else tail = t->prev;
        if (current == t) current = t->next ? t->next : t->prev;
        delete t;
        cout << "Song deleted.\n";
    }
    void show(Song* t) {
        cout << t->id << " | " << t->name << " | " << t->min << ":" << (t->sec < 10 ? "0" : "") << t->sec << endl;
    }
    // 3. Display forward
    void displayForward() {
        if (!head) { cout << "Playlist empty.\n"; return; }
        for (Song* t = head; t; t = t->next) show(t);
    }
    // 4. Display backward
    void displayBackward() {
        if (!tail) { cout << "Playlist empty.\n"; return; }
        for (Song* t = tail; t; t = t->prev) show(t);
    }
    // 5. Search by id
    void search(int id) {
        Song* t = find(id);
        if (t) show(t);
        else cout << "Song not found.\n";
    }
    // 6. Play next / previous
    void playNext() {
        if (!head) { cout << "Playlist empty.\n"; return; }
        if (!current) current = head;
        else if (current->next) current = current->next;
        else { cout << "Already at last song.\n"; }
        cout << "Playing: "; show(current);
    }
    void playPrev() {
        if (!head) { cout << "Playlist empty.\n"; return; }
        if (!current) current = tail;
        else if (current->prev) current = current->prev;
        else { cout << "Already at first song.\n"; }
        cout << "Playing: "; show(current);
    }
    // 7. Reverse in place (swap prev and next of every node)
    void reverse() {
        Song* t = head;
        while (t) {
            Song* temp = t->prev;
            t->prev = t->next;
            t->next = temp;
            t = t->prev; // old next
        }
        Song* temp = head;
        head = tail;
        tail = temp;
        cout << "Playlist reversed.\n";
    }
};
int main() {
    Playlist p;
    int ch, id, m, s;
    string name;
    do {
        cout << "\n1.Add 2.Delete 3.Forward 4.Backward 5.Search 6.Next 7.Previous 8.Reverse 0.Exit\nChoice: ";
        cin >> ch;
        if (ch == 1) {
            cout << "ID: "; cin >> id;
            cout << "Name: "; cin.ignore(); getline(cin, name);
            cout << "Duration (min sec): "; cin >> m >> s;
            p.addSong(id, name, m, s);
        }
        else if (ch == 2) { cout << "ID: "; cin >> id; p.deleteSong(id); }
        else if (ch == 3) p.displayForward();
        else if (ch == 4) p.displayBackward();
        else if (ch == 5) { cout << "ID: "; cin >> id; p.search(id); }
        else if (ch == 6) p.playNext();
        else if (ch == 7) p.playPrev();
        else if (ch == 8) p.reverse();
    } while (ch != 0);
    return 0;
}