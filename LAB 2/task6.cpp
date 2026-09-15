#include <iostream>
using namespace std;
int main() {
    int rows, cols;
    cout << "Enter number of students: ";
    cin >> rows;
    cout << "Enter number of subjects: ";
    cin >> cols;
    // make sure rows and cols are valid
    while (rows <= 0 || cols <= 0) {
        cout << "Invalid input. Enter again: ";
        cin >> rows >> cols;
    }
    // create array of row pointers
    int **marks = new int*[rows];
    // create each row
    for (int i = 0; i < rows; i++) {
        marks[i] = new int[cols];}
    // take input
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << "Student " << i + 1 << " Subject " << j + 1 << " marks: ";
            cin >> *(*(marks + i) + j);}}
    // show the matrix
    cout << "\nMarks Matrix:\n";
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << marks[i][j] << " ";}
        cout << endl;}
    // find total marks of first student
    int bestTotal = 0;
    for (int j = 0; j < cols; j++) {
        bestTotal = bestTotal + marks[0][j];}
    int bestStudent = 1;
    // check other students
    for (int i = 1; i < rows; i++) {
        int total = 0;
        for (int j = 0; j < cols; j++) {
            total = total + marks[i][j];}
        if (total > bestTotal) {
            bestTotal = total;
            bestStudent = i + 1;
        }}
    cout << "\nHighest total is " << bestTotal << " by Student " << bestStudent << endl;
    // delete memory
    for (int i = 0; i < rows; i++) {
        delete[] marks[i];
    }
    delete[] marks;
    marks = nullptr;
    return 0;
}