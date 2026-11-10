// Lab 9 - Task 2: Word Frequency Indexing using BST
#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct WordNode {
    string word;
    int freq;
    WordNode *left, *right;
    WordNode(const string& w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class WordBST {
    WordNode* root = nullptr;
    int uniqueCount = 0, totalCount = 0;

    WordNode* insert(WordNode* n, const string& w) {
        if (!n) { uniqueCount++; return new WordNode(w); }
        if (w < n->word) n->left = insert(n->left, w);
        else if (w > n->word) n->right = insert(n->right, w);
        else n->freq++;                       // word already exists
        return n;
    }
    void inorder(WordNode* n, vector<WordNode*>& out) const {
        if (!n) return;
        inorder(n->left, out);
        out.push_back(n);
        inorder(n->right, out);
    }
    void destroy(WordNode* n) {
        if (!n) return;
        destroy(n->left); destroy(n->right); delete n;
    }

public:
    ~WordBST() { destroy(root); }

    void insert(string w) {
        for (char& c : w) c = tolower((unsigned char)c);   // ignore case
        if (w.empty()) return;
        totalCount++;
        root = insert(root, w);
    }

    // Parse paragraph: strip punctuation, split on whitespace
    void insertParagraph(const string& text) {
        string cleaned;
        for (char c : text) cleaned += isalnum((unsigned char)c) ? c : ' ';
        stringstream ss(cleaned);
        string w;
        while (ss >> w) insert(w);
    }

    void displayAlphabetical() const {
        vector<WordNode*> v; inorder(root, v);
        cout << "Alphabetical order:\n";
        for (auto* n : v) cout << "  " << n->word << " : " << n->freq << "\n";
    }

    void displayByFrequency() const {
        vector<WordNode*> v; inorder(root, v);   // already alphabetical -> stable tie-break
        stable_sort(v.begin(), v.end(),
                    [](WordNode* a, WordNode* b) { return a->freq > b->freq; });
        cout << "By frequency (high to low):\n";
        for (auto* n : v) cout << "  " << n->word << " : " << n->freq << "\n";
    }

    void findMostFrequentWord() const {
        vector<WordNode*> v; inorder(root, v);
        int best = 0;
        for (auto* n : v) best = max(best, n->freq);
        cout << "Most frequent word(s) (count = " << best << "): ";
        for (auto* n : v) if (n->freq == best) cout << n->word << " ";
        cout << "\n";
    }

    void summary() const {
        cout << "Total unique words: " << uniqueCount << "\n";
        cout << "Total word count  : " << totalCount << "\n";
    }
};

int main() {
    string paragraph =
        "The quick brown fox jumps over the lazy dog. The fox is quick and smart.";

    WordBST tree;
    tree.insertParagraph(paragraph);

    tree.displayAlphabetical();
    cout << "\n";
    tree.displayByFrequency();
    cout << "\n";
    tree.findMostFrequentWord();
    cout << "\n";
    tree.summary();
    return 0;
}