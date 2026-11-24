// Lab 11 - Task 1: Huffman coding using a binary (min) heap
// Build the Huffman tree, then print every character's code in preorder.

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    int order;          // insertion order, breaks ties so output is deterministic
    Node* left;
    Node* right;
    Node(char c, int f, int o) : ch(c), freq(f), order(o), left(nullptr), right(nullptr) {}
};

// Simple array-based min heap of Node pointers
class MinHeap {
    vector<Node*> a;

    bool less(Node* x, Node* y) {
        if (x->freq != y->freq) return x->freq < y->freq;
        return x->order < y->order;
    }
    void heapifyUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (less(a[i], a[p])) { swap(a[i], a[p]); i = p; }
            else break;
        }
    }
    void heapifyDown(int i) {
        int n = a.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < n && less(a[l], a[s])) s = l;
            if (r < n && less(a[r], a[s])) s = r;
            if (s == i) break;
            swap(a[i], a[s]);
            i = s;
        }
    }
public:
    int size() { return a.size(); }
    void push(Node* x) { a.push_back(x); heapifyUp(a.size() - 1); }
    Node* pop() {
        Node* top = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) heapifyDown(0);
        return top;
    }
};

Node* buildHuffman(const string& S, const vector<int>& f) {
    MinHeap h;
    int order = 0;
    for (int i = 0; i < (int)S.size(); i++)
        h.push(new Node(S[i], f[i], order++));

    // Repeatedly merge the two smallest nodes
    while (h.size() > 1) {
        Node* l = h.pop();
        Node* r = h.pop();
        Node* parent = new Node('$', l->freq + r->freq, order++);
        parent->left = l;
        parent->right = r;
        h.push(parent);
    }
    return h.pop();
}

// Preorder: left edge = '0', right edge = '1'
void printCodes(Node* root, string code) {
    if (!root) return;
    if (!root->left && !root->right) {
        cout << root->ch << " : " << (code.empty() ? "0" : code) << endl;
        return;
    }
    printCodes(root->left, code + "0");
    printCodes(root->right, code + "1");
}

void freeTree(Node* root) {
    if (!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    string S = "abcdef";
    vector<int> f = {5, 9, 12, 13, 16, 45};

    Node* root = buildHuffman(S, f);
    cout << "Huffman codes (preorder):" << endl;
    printCodes(root, "");
    freeTree(root);
    return 0;
}
