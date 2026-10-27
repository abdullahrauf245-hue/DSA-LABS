// Lab 8 - Task 1: Peak element in a 2D array
// Approach: binary search on columns -> O(n log m), i.e. O(n log n) for an n x n matrix

#include <iostream>
#include <vector>
using namespace std;

// Returns {row, col} of a peak element (>= all its up/down/left/right neighbors)
pair<int, int> findPeak2D(const vector<vector<int>>& A) {
    int rows = A.size();
    int cols = A[0].size();
    int low = 0, high = cols - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;

        // Step 1: find the row of the maximum element in the middle column -> O(n)
        int maxRow = 0;
        for (int i = 1; i < rows; i++) {
            if (A[i][mid] > A[maxRow][mid])
                maxRow = i;
        }

        // Step 2: compare with left and right neighbors (treat out-of-bounds as -infinity)
        bool leftOk  = (mid == 0)        || A[maxRow][mid] >= A[maxRow][mid - 1];
        bool rightOk = (mid == cols - 1) || A[maxRow][mid] >= A[maxRow][mid + 1];

        if (leftOk && rightOk)
            return {maxRow, mid};          // found a peak
        else if (!rightOk)
            low = mid + 1;                 // bigger neighbor on the right -> go right
        else
            high = mid - 1;                // bigger neighbor on the left -> go left
    }
    return {-1, -1};                       // never reached for a non-empty matrix
}

int main() {
    vector<vector<int>> A = {
        {10, 8, 10, 10},
        {14, 13, 12, 11},
        {15, 9, 11, 21},
        {16, 17, 19, 20}
    };

    pair<int, int> p = findPeak2D(A);
    cout << "Peak element: " << A[p.first][p.second]
         << " at (row " << p.first << ", col " << p.second << ")\n";
    return 0;
}