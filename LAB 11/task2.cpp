// Lab 11 - Task 2: Merge k sorted linked lists using a binary (min) heap
// Time: O(N log k), N = total nodes. Space: O(k) for the heap.

#include <iostream>
#include <vector>
using namespace std;

struct ListNode {
    int val;
    ListNode* next;
    ListNode(int v) : val(v), next(nullptr) {}
};

// Array-based min heap that stores list nodes, ordered by val
class MinHeap {
    vector<ListNode*> a;

    void heapifyUp(int i) {
        while (i > 0) {
            int p = (i - 1) / 2;
            if (a[i]->val < a[p]->val) { swap(a[i], a[p]); i = p; }
            else break;
        }
    }
    void heapifyDown(int i) {
        int n = a.size();
        while (true) {
            int l = 2 * i + 1, r = 2 * i + 2, s = i;
            if (l < n && a[l]->val < a[s]->val) s = l;
            if (r < n && a[r]->val < a[s]->val) s = r;
            if (s == i) break;
            swap(a[i], a[s]);
            i = s;
        }
    }
public:
    bool empty() { return a.empty(); }
    void push(ListNode* x) { a.push_back(x); heapifyUp(a.size() - 1); }
    ListNode* pop() {
        ListNode* top = a[0];
        a[0] = a.back();
        a.pop_back();
        if (!a.empty()) heapifyDown(0);
        return top;
    }
};

ListNode* mergeKLists(vector<ListNode*>& lists) {
    MinHeap h;
    // Put the head of every list into the heap
    for (ListNode* head : lists)
        if (head) h.push(head);

    ListNode dummy(0);
    ListNode* tail = &dummy;

    // Take the smallest, attach it, then push its successor
    while (!h.empty()) {
        ListNode* smallest = h.pop();
        tail->next = smallest;
        tail = smallest;
        if (smallest->next) h.push(smallest->next);
    }
    tail->next = nullptr;
    return dummy.next;
}

ListNode* buildList(const vector<int>& v) {
    ListNode dummy(0);
    ListNode* t = &dummy;
    for (int x : v) { t->next = new ListNode(x); t = t->next; }
    return dummy.next;
}

void printList(ListNode* head) {
    cout << "[";
    while (head) {
        cout << head->val;
        if (head->next) cout << " -> ";
        head = head->next;
    }
    cout << "]" << endl;
}

void freeList(ListNode* head) {
    while (head) { ListNode* n = head->next; delete head; head = n; }
}

int main() {
    vector<ListNode*> lists = {
        buildList({1, 4, 5}),
        buildList({1, 3, 4}),
        buildList({2, 6}),
        buildList({})
    };

    cout << "Input lists:" << endl;
    for (ListNode* l : lists) printList(l);

    ListNode* merged = mergeKLists(lists);
    cout << "Merged list:" << endl;
    printList(merged);

    freeList(merged);
    return 0;
}
