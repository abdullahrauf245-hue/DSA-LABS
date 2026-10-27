// Lab 8 - Task 2: Knight's Tour
// Part 1: plain backtracking
// Part 2: optimized with Warnsdorff's rule (always move to the square with the fewest onward moves)

#include <iostream>
#include <vector>
#include <iomanip>
using namespace std;

const int dx[8] = {2, 1, -1, -2, -2, -1, 1, 2};
const int dy[8] = {1, 2, 2, 1, -1, -2, -2, -1};

bool isSafe(int x, int y, int N, const vector<vector<int>>& board) {
    return x >= 0 && y >= 0 && x < N && y < N && board[x][y] == -1;
}

void printBoard(const vector<vector<int>>& board) {
    for (auto& row : board) {
        for (int v : row) cout << setw(4) << v;
        cout << "\n";
    }
}

// ---------- Part 1: Backtracking ----------
// Worst case: O(8^(N^2)) (exponential). Best case: O(N^2) when the very first
// choice at every step works out and no backtracking is needed.
bool solveBacktrack(int x, int y, int move, int N, vector<vector<int>>& board) {
    if (move == N * N) return true;
    for (int k = 0; k < 8; k++) {
        int nx = x + dx[k], ny = y + dy[k];
        if (isSafe(nx, ny, N, board)) {
            board[nx][ny] = move;
            if (solveBacktrack(nx, ny, move + 1, N, board)) return true;
            board[nx][ny] = -1;  // backtrack
        }
    }
    return false;
}

// ---------- Part 2: Warnsdorff optimization ----------
int degree(int x, int y, int N, const vector<vector<int>>& board) {
    int count = 0;
    for (int k = 0; k < 8; k++)
        if (isSafe(x + dx[k], y + dy[k], N, board)) count++;
    return count;
}

// N^2 squares, each step checks 8 neighbors x 8 moves = constant work -> O(N^2)
// (Warnsdorff is a heuristic: it almost always works with no backtracking,
//  but it is not guaranteed for every start square on every board size.)
bool solveWarnsdorff(int sx, int sy, int N, vector<vector<int>>& board) {
    int x = sx, y = sy;
    board[x][y] = 0;
    for (int move = 1; move < N * N; move++) {
        int best = -1, minDeg = 9;
        for (int k = 0; k < 8; k++) {
            int nx = x + dx[k], ny = y + dy[k];
            if (isSafe(nx, ny, N, board)) {
                int d = degree(nx, ny, N, board);
                if (d < minDeg) { minDeg = d; best = k; }
            }
        }
        if (best == -1) return false;  // stuck
        x += dx[best];
        y += dy[best];
        board[x][y] = move;
    }
    return true;
}

int main() {
    int N, sx, sy;
    cout << "Enter board size N: ";
    cin >> N;
    cout << "Enter starting position (x y): ";
    cin >> sx >> sy;

    if (sx < 0 || sy < 0 || sx >= N || sy >= N) {
        cout << "Invalid starting position.\n";
        return 1;
    }

    // Backtracking (only practical for small N, e.g. N <= 6)
    if (N <= 6) {
        vector<vector<int>> b1(N, vector<int>(N, -1));
        b1[sx][sy] = 0;
        cout << "\n--- Backtracking ---\n";
        if (solveBacktrack(sx, sy, 1, N, b1)) printBoard(b1);
        else cout << "No tour found.\n";
    }

    // Optimized
    vector<vector<int>> b2(N, vector<int>(N, -1));
    cout << "\n--- Optimized (Warnsdorff) ---\n";
    if (solveWarnsdorff(sx, sy, N, b2)) printBoard(b2);
    else cout << "No tour found.\n";

    return 0;
}