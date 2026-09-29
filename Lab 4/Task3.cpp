#include <iostream>
#include <string>
using namespace std;
// One bit per node. Head = MSB, tail = LSB
struct Node {
    int bit;
    Node* next;
    Node* prev;
};
class Bin {
public:
    Node* head;
    Node* tail;
    int size;
    Bin() { head = tail = NULL; size = 0; }
    Bin(const Bin& o) { head = tail = NULL; size = 0; copy(o); }
    Bin& operator=(const Bin& o) {
        if (this != &o) { clear(); copy(o); }
        return *this;
    }
    ~Bin() { clear(); }
    void copy(const Bin& o) {
        for (Node* t = o.head; t; t = t->next) pushBack(t->bit);
    }
    void pushBack(int b) {
        Node* n = new Node{b, NULL, tail};
        if (tail) tail->next = n;
        else head = n;
        tail = n;
        size++;
    }
    void pushFront(int b) {
        Node* n = new Node{b, head, NULL};
        if (head) head->prev = n;
        else tail = n;
        head = n;
        size++;
    }
    void clear() {
        while (head) {
            Node* t = head;
            head = head->next;
            delete t;
        }
        tail = NULL;
        size = 0;
    }
    // add leading zeros until length is a multiple of 8
    void pad() { while (size % 8 != 0) pushFront(0); }
    // 1. Store binary number
    void store(string s) {
        clear();
        for (char c : s) pushBack(c - '0');
        pad();
    }
    void print() {
        int i = 0;
        for (Node* t = head; t; t = t->next) {
            cout << t->bit;
            i++;
            if (i % 8 == 0 && t->next) cout << " ";
        }
        cout << endl;
    }
    // 2. One's complement
    void ones() {
        for (Node* t = head; t; t = t->next) t->bit = 1 - t->bit;
    }
    // 3. Two's complement = ones + 1 (carry goes from tail to head)
    void twos() {
        ones();
        int carry = 1;
        for (Node* t = tail; t; t = t->prev) {
            int s = t->bit + carry;
            t->bit = s % 2;
            carry = s / 2;
        }
    }
    // 6. Convert to decimal
    long long toDecimal() {
        long long v = 0;
        for (Node* t = head; t; t = t->next) v = v * 2 + t->bit;
        return v;
    }
};
// 4. Binary addition: go from LSB to MSB with carry
Bin add(Bin& a, Bin& b) {
    Bin r;
    Node* x = a.tail;
    Node* y = b.tail;
    int c = 0;
    while (x || y || c) {
        int s = c;
        if (x) { s += x->bit; x = x->prev; }
        if (y) { s += y->bit; y = y->prev; }
        r.pushFront(s % 2);
        c = s / 2;
    }
    r.pad();
    return r;
}
// 5. Multiplication: repeated addition + shifting
Bin multiply(Bin& a, Bin& b) {
    Bin r;
    r.store("0");
    Bin shifted = a;
    for (Node* y = b.tail; y; y = y->prev) {
        if (y->bit == 1) r = add(r, shifted);
        shifted.pushBack(0); // shift left by one
    }
    return r;
}
bool valid(string s) {
    if (s.empty()) return false;
    for (char c : s) if (c != '0' && c != '1') return false;
    return true;
}
int main() {
    string s1, s2;
    Bin a, b;
    int ch;
    cout << "Enter first binary number: "; cin >> s1;
    cout << "Enter second binary number: "; cin >> s2;
    if (!valid(s1) || !valid(s2)) { cout << "Invalid binary input.\n"; return 0; }
    a.store(s1);
    b.store(s2);
    do {
        cout << "\n1.Show numbers 2.Ones complement 3.Twos complement 4.Add 5.Multiply 6.To decimal 0.Exit\nChoice: ";
        cin >> ch;
        if (ch == 1) { cout << "A = "; a.print(); cout << "B = "; b.print(); }
        else if (ch == 2) {
            Bin t = a; t.ones();
            cout << "1's complement of A: "; t.print();
        }
        else if (ch == 3) {
            Bin t = a; t.twos();
            cout << "2's complement of A: "; t.print();
        }
        else if (ch == 4) {
            Bin r = add(a, b);
            cout << "A + B = "; r.print();
            cout << "Decimal: " << r.toDecimal() << endl;
        }
        else if (ch == 5) {
            Bin r = multiply(a, b);
            cout << "A * B = "; r.print();
            cout << "Decimal: " << r.toDecimal() << endl;
        }
        else if (ch == 6) {
            cout << "A = " << a.toDecimal() << ", B = " << b.toDecimal() << endl;
        }
    } while (ch != 0);
    return 0;
}