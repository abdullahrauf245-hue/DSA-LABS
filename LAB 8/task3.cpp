// Lab 8 - Task 3: Iterative Tower of Hanoi
// Idea: total moves = 2^N - 1. Pegs: S (source), A (auxiliary), D (destination).
// If N is even, swap the roles of D and A. Then for each move i = 1 .. 2^N - 1:
//   i % 3 == 1 -> legal move between S and D
//   i % 3 == 2 -> legal move between S and A
//   i % 3 == 0 -> legal move between A and D
// Time complexity: O(2^N) (one constant-time step per move). Space: O(N) for the stacks.

#include <iostream>
#include <stack>
#include <cmath>
using namespace std;

// Move the legal direction between two pegs (the smaller top disk goes onto the other peg)
void moveDisk(stack<int>& from, stack<int>& to, char f, char t) {
    if (from.empty() && to.empty()) return;

    if (from.empty()) {
        int d = to.top(); to.pop(); from.push(d);
        cout << "Move disk " << d << " from " << t << " to " << f << "\n";
    } else if (to.empty()) {
        int d = from.top(); from.pop(); to.push(d);
        cout << "Move disk " << d << " from " << f << " to " << t << "\n";
    } else if (from.top() < to.top()) {
        int d = from.top(); from.pop(); to.push(d);
        cout << "Move disk " << d << " from " << f << " to " << t << "\n";
    } else {
        int d = to.top(); to.pop(); from.push(d);
        cout << "Move disk " << d << " from " << t << " to " << f << "\n";
    }
}

void hanoiIterative(int N) {
    stack<int> src, aux, dest;
    char s = 'S', a = 'A', d = 'D';

    if (N % 2 == 0) swap(d, a);  // even N: swap destination and auxiliary

    for (int i = N; i >= 1; i--) src.push(i);

    // Pegs stored by name so swapped roles still map to the right stack
    auto getStack = [&](char c) -> stack<int>& {
        if (c == 'S') return src;
        if (c == 'A') return aux;
        return dest;
    };

    long long totalMoves = (1LL << N) - 1;
    for (long long i = 1; i <= totalMoves; i++) {
        if (i % 3 == 1)      moveDisk(getStack(s), getStack(d), s, d);
        else if (i % 3 == 2) moveDisk(getStack(s), getStack(a), s, a);
        else                 moveDisk(getStack(a), getStack(d), a, d);
    }
}

int main() {
    int N;
    cout << "Enter number of disks: ";
    cin >> N;
    cout << "\nMoves (Source = S, Auxiliary = A, Destination = D):\n";
    hanoiIterative(N);
    cout << "\nTotal moves: " << ((1LL << N) - 1) << "\n";
    return 0;
}