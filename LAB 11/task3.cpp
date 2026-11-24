// Lab 11 - Task 3: Self-balancing AVL tree with top-k queries
// Supports insert, delete, update, and top-k by a criterion (max / min / sum).
// The tree rebalances itself (rotations only when needed) after every operation.

#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node {
    int key;
    int height;
    Node* left;
    Node* right;
    Node(int k) : key(k), height(1), left(nullptr), right(nullptr) {}
};

enum Criterion { MAXIMUM, MINIMUM, SUM };

class AVLTree {
    Node* root = nullptr;

    int height(Node* n) { return n ? n->height : 0; }
    int balance(Node* n) { return n ? height(n->left) - height(n->right) : 0; }
    void update(Node* n) { n->height = 1 + max(height(n->left), height(n->right)); }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        y->left = x->right;
        x->right = y;
        update(y);
        update(x);
        return x;
    }
    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        x->right = y->left;
        y->left = x;
        update(x);
        update(y);
        return y;
    }

    // "Heal" a node: rotate only if its balance factor is outside [-1, 1]
    Node* rebalance(Node* n) {
        update(n);
        int bf = balance(n);
        if (bf > 1) {
            if (balance(n->left) < 0) {                 // LR case
                cout << "  Rebalance at " << n->key << " (LR rotation)" << endl;
                n->left = rotateLeft(n->left);
            } else {
                cout << "  Rebalance at " << n->key << " (LL rotation)" << endl;
            }
            return rotateRight(n);
        }
        if (bf < -1) {
            if (balance(n->right) > 0) {                // RL case
                cout << "  Rebalance at " << n->key << " (RL rotation)" << endl;
                n->right = rotateRight(n->right);
            } else {
                cout << "  Rebalance at " << n->key << " (RR rotation)" << endl;
            }
            return rotateLeft(n);
        }
        return n;                                       // already balanced
    }

    Node* insert(Node* n, int key) {
        if (!n) return new Node(key);
        if (key < n->key) n->left = insert(n->left, key);
        else if (key > n->key) n->right = insert(n->right, key);
        else return n;                                  // duplicates ignored
        return rebalance(n);
    }

    Node* minNode(Node* n) {
        while (n->left) n = n->left;
        return n;
    }

    Node* remove(Node* n, int key, bool& found) {
        if (!n) return nullptr;
        if (key < n->key) n->left = remove(n->left, key, found);
        else if (key > n->key) n->right = remove(n->right, key, found);
        else {
            found = true;
            if (!n->left || !n->right) {
                Node* child = n->left ? n->left : n->right;
                delete n;
                return child;
            }
            Node* succ = minNode(n->right);             // inorder successor
            n->key = succ->key;
            n->right = remove(n->right, succ->key, found);
        }
        return rebalance(n);
    }

    bool contains(Node* n, int key) {
        while (n) {
            if (key == n->key) return true;
            n = key < n->key ? n->left : n->right;
        }
        return false;
    }

    // Reverse inorder (right, root, left) -> largest first; stops after k
    void largestK(Node* n, int k, vector<int>& out) {
        if (!n || (int)out.size() >= k) return;
        largestK(n->right, k, out);
        if ((int)out.size() < k) out.push_back(n->key);
        largestK(n->left, k, out);
    }
    // Inorder -> smallest first; stops after k
    void smallestK(Node* n, int k, vector<int>& out) {
        if (!n || (int)out.size() >= k) return;
        smallestK(n->left, k, out);
        if ((int)out.size() < k) out.push_back(n->key);
        smallestK(n->right, k, out);
    }

    // Sideways print: right subtree on top, root on the left
    void print(Node* n, string indent) {
        if (!n) return;
        print(n->right, indent + "      ");
        cout << indent << n->key << "(h" << n->height << ")" << endl;
        print(n->left, indent + "      ");
    }

    void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

public:
    ~AVLTree() { destroy(root); }

    void insert(int key) {
        cout << "Insert " << key << endl;
        root = insert(root, key);
        printTree();
    }

    void insertMany(const vector<int>& keys) {
        for (int k : keys) insert(k);
    }

    bool remove(int key) {
        bool found = false;
        cout << "Delete " << key << endl;
        root = remove(root, key, found);
        if (!found) cout << "  " << key << " not found" << endl;
        printTree();
        return found;
    }

    // Update = delete old value + insert new value (both self-balance)
    void updateKey(int oldKey, int newKey) {
        cout << "Update " << oldKey << " -> " << newKey << endl;
        if (!contains(root, oldKey)) { cout << "  " << oldKey << " not found" << endl; return; }
        bool found = false;
        root = remove(root, oldKey, found);
        root = insert(root, newKey);
        printTree();
    }

    // Returns the top-k elements for MAXIMUM / MINIMUM.
    // For SUM, returns the k elements whose sum is largest (the k largest keys).
    vector<int> topK(int k, Criterion c) {
        vector<int> out;
        if (c == MINIMUM) smallestK(root, k, out);
        else largestK(root, k, out);
        return out;
    }

    void queryTopK(int k, Criterion c) {
        vector<int> res = topK(k, c);
        string name = c == MAXIMUM ? "maximum" : c == MINIMUM ? "minimum" : "sum";
        cout << "Top " << k << " by " << name << ": ";
        long long sum = 0;
        for (int x : res) { cout << x << " "; sum += x; }
        if (c == SUM) cout << " => sum = " << sum;
        cout << endl;
    }

    void deleteTopK(int k, Criterion c) {
        vector<int> res = topK(k, c);
        cout << "Deleting top " << k << " elements..." << endl;
        for (int x : res) remove(x);
    }

    void printTree() {
        if (!root) { cout << "  (empty tree)" << endl; return; }
        print(root, "  ");
        cout << "  ------------------------" << endl;
    }
};

int main() {
    AVLTree t;

    // 1. Insert multiple elements (tree prints after each, with rotations shown)
    t.insertMany({10, 20, 30, 40, 50, 25, 5, 4, 35});

    // 2. Query top k for each criterion
    int k = 3;
    t.queryTopK(k, MAXIMUM);
    t.queryTopK(k, MINIMUM);
    t.queryTopK(k, SUM);

    // 3. Update an element
    t.updateKey(4, 45);
    t.queryTopK(k, MAXIMUM);

    // 4. Delete top k (maximum) elements, tree printed after each deletion
    t.deleteTopK(k, MAXIMUM);
    t.queryTopK(k, MAXIMUM);

    // 5. Delete top k (minimum) elements
    t.deleteTopK(k, MINIMUM);
    return 0;
}
