// CS250 - Lab 8, Task 1: Binary Tree Representation and Operations
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    char data;
    Node *left, *right;
    Node(char c) : data(c), left(nullptr), right(nullptr) {}
};

/* ---------- 1. Build tree from string: a(b(d,e),c(,f(g,))) ---------- */
// grammar: tree := label [ '(' [tree] ',' [tree] ')' ]
Node* build(const string& s, int& i) {
    if (i >= (int)s.size() || s[i] == ',' || s[i] == ')') return nullptr; // empty child
    Node* root = new Node(s[i++]);
    if (i < (int)s.size() && s[i] == '(') {
        i++;                         // skip '('
        root->left = build(s, i);
        i++;                         // skip ','
        root->right = build(s, i);
        i++;                         // skip ')'
    }
    return root;
}
Node* buildTree(const string& s) { int i = 0; return build(s, i); }

/* ---------- 2. Preorder string form (recursive) ---------- */
string preorderString(Node* r) {
    if (!r) return "";
    string out(1, r->data);
    if (r->left || r->right)
        out += "(" + preorderString(r->left) + "," + preorderString(r->right) + ")";
    return out;
}
void preorderPlain(Node* r) {
    if (!r) return;
    cout << r->data << " ";
    preorderPlain(r->left);
    preorderPlain(r->right);
}

/* ---------- 3. Inorder (iterative, stack) ---------- */
string inorderIter(Node* r) {
    string out; stack<Node*> st; Node* cur = r;
    while (cur || !st.empty()) {
        while (cur) { st.push(cur); cur = cur->left; }
        cur = st.top(); st.pop();
        out += cur->data; out += ' ';
        cur = cur->right;
    }
    return out;
}

/* ---------- 4. Postorder (iterative, stack) ---------- */
string postorderIter(Node* r) {
    string out; stack<Node*> st; Node *cur = r, *last = nullptr;
    while (cur || !st.empty()) {
        if (cur) { st.push(cur); cur = cur->left; }
        else {
            Node* top = st.top();
            if (top->right && last != top->right) cur = top->right;
            else { out += top->data; out += ' '; last = top; st.pop(); }
        }
    }
    return out;
}

/* ---------- 5. Validate string WITHOUT building the tree ---------- */
bool validTree(const string& s, int& i) {
    if (i >= (int)s.size()) return false;
    if (s[i] == ',' || s[i] == ')') return true;        // empty child is OK
    if (!isalnum((unsigned char)s[i])) return false;    // label must be alnum
    i++;
    if (i < (int)s.size() && s[i] == '(') {
        i++;
        if (!validTree(s, i)) return false;             // left
        if (i >= (int)s.size() || s[i] != ',') return false;
        i++;
        if (!validTree(s, i)) return false;             // right
        if (i >= (int)s.size() || s[i] != ')') return false;
        i++;
    }
    return true;
}
bool isValid(const string& s) {
    if (s.empty()) return false;
    int bal = 0;                                        // quick parentheses check
    for (char c : s) {
        if (c == '(') bal++;
        else if (c == ')' && --bal < 0) return false;
    }
    if (bal != 0) return false;
    int i = 0;
    return validTree(s, i) && i == (int)s.size();
}

/* ---------- 6. Height and depth ---------- */
int height(Node* r) {                                   // edges; empty tree = -1
    if (!r) return -1;
    return 1 + max(height(r->left), height(r->right));
}
void printDepths(Node* r, int d = 0) {
    if (!r) return;
    cout << "  Node " << r->data << " -> depth " << d << "\n";
    printDepths(r->left, d + 1);
    printDepths(r->right, d + 1);
}

/* ---------- 7. BST from integer sequence ---------- */
struct BNode {
    int val; BNode *left, *right;
    BNode(int v) : val(v), left(nullptr), right(nullptr) {}
};
BNode* insertBST(BNode* r, int v) {
    if (!r) return new BNode(v);
    if (v < r->val) r->left = insertBST(r->left, v);
    else if (v > r->val) r->right = insertBST(r->right, v);   // duplicates ignored
    return r;
}
BNode* buildBST(const vector<int>& a) {
    BNode* root = nullptr;
    for (int x : a) root = insertBST(root, x);          // first element = root
    return root;
}
void inorderBST(BNode* r) {
    if (!r) return;
    inorderBST(r->left); cout << r->val << " "; inorderBST(r->right);
}

void freeTree(Node* r) { if (r) { freeTree(r->left); freeTree(r->right); delete r; } }
void freeBST(BNode* r) { if (r) { freeBST(r->left); freeBST(r->right); delete r; } }

int main() {
    string s = "a(b(d,e),c(,f(g,)))";
    cout << "Input string: " << s << "\n\n";

    cout << "[5] Valid string? " << (isValid(s) ? "YES" : "NO") << "\n";
    cout << "    Test \"a(b(d,e),c(,f(g,))\" -> " << (isValid("a(b(d,e),c(,f(g,))") ? "YES" : "NO") << "\n";
    cout << "    Test \"a(b,c,d)\"           -> " << (isValid("a(b,c,d)") ? "YES" : "NO") << "\n\n";

    if (!isValid(s)) { cout << "Invalid input, stopping.\n"; return 0; }

    Node* root = buildTree(s);                          // [1]
    cout << "[1] Tree built from string.\n\n";

    cout << "[2] Preorder (string form): " << preorderString(root) << "\n";
    cout << "    Preorder (sequence)   : "; preorderPlain(root); cout << "\n\n";
    cout << "[3] Inorder   (stack)     : " << inorderIter(root) << "\n";
    cout << "[4] Postorder (stack)     : " << postorderIter(root) << "\n\n";

    cout << "[6] Height of tree: " << height(root) << "\n";
    cout << "    Depth of each node:\n";
    printDepths(root);

    vector<int> v = {50, 30, 70, 20, 40, 60, 80};
    BNode* bst = buildBST(v);
    cout << "\n[7] BST inorder: "; inorderBST(bst); cout << "\n";

    freeTree(root); freeBST(bst);
    return 0;
}