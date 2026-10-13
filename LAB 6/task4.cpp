/*
 * CS-250 Data Structures and Algorithms - Lab 6
 * Task 4: Waiting Room Management System (emergency ward)
 *
 * Patients are registered first-come-first-served and receive an auto-generated
 * unique ID starting from 1. The waiting line is an array-based circular queue
 * that doubles its capacity when full, so registration never fails.
 *
 * Methods (as required by the lab)
 *   RegisterPatient()  - assign the next ID and enqueue the patient
 *   ServePatient()     - dequeue the patient who has waited longest
 *   CancelAll()        - remove every waiting patient (doctor goes to lunch)
 *   CanDoctorGoHome()  - true if nobody is waiting
 *   ShowAllPatient()   - print waiting IDs in SORTED (ascending) order
 *
 * Design note: IDs are never reused. After CancelAll() the next patient still
 * gets a brand new ID, so an ID always identifies exactly one patient.
 *
 * Compile : g++ -std=c++17 -Wall -Wextra task4_waiting_room.cpp -o task4
 * Run     : ./task4
 */

#include <iostream>
#include <string>

using namespace std;

class WaitingRoom {
    int* data_;    // circular array of patient IDs
    int cap_;
    int head_;     // index of the front patient
    int size_;     // number of patients waiting
    int nextId_;   // auto-generated ID counter

    // Double the array when it is full, copying in queue order.
    void grow() {
        int* fresh = new int[cap_ * 2];
        for (int k = 0; k < size_; ++k) fresh[k] = data_[(head_ + k) % cap_];
        delete[] data_;
        data_ = fresh;
        head_ = 0;
        cap_ *= 2;
    }

    // Sort a plain int array ascending (insertion sort).
    static void insertionSort(int* a, int n) {
        for (int i = 1; i < n; ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= 0 && a[j] > key) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = key;
        }
    }

public:
    explicit WaitingRoom(int capacity = 5)
        : data_(new int[capacity]), cap_(capacity), head_(0), size_(0), nextId_(1) {}
    ~WaitingRoom() { delete[] data_; }
    WaitingRoom(const WaitingRoom&) = delete;
    WaitingRoom& operator=(const WaitingRoom&) = delete;

    // Assign the next ID to a new patient and add them to the queue.
    // Returns the assigned ID.
    int RegisterPatient() {
        if (size_ == cap_) grow();
        int id = nextId_++;
        data_[(head_ + size_) % cap_] = id;
        ++size_;
        return id;
    }

    // Call the patient at the front of the queue.
    // Returns their ID, or 0 if nobody is waiting (valid IDs start at 1).
    int ServePatient() {
        if (size_ == 0) return 0;
        int id = data_[head_];
        head_ = (head_ + 1) % cap_;
        --size_;
        return id;
    }

    // Cancel every waiting appointment. Returns how many were cancelled.
    int CancelAll() {
        int cancelled = size_;
        head_ = 0;
        size_ = 0;
        return cancelled;
    }

    // True if no one is waiting, otherwise false.
    bool CanDoctorGoHome() const { return size_ == 0; }

    // Print the IDs of all waiting patients in sorted order.
    // Works on a temporary copy so the queue itself is not disturbed.
    void ShowAllPatient() const {
        if (size_ == 0) {
            cout << "No patients are waiting.\n";
            return;
        }
        int* tmp = new int[size_];
        for (int k = 0; k < size_; ++k) tmp[k] = data_[(head_ + k) % cap_];
        insertionSort(tmp, size_);
        cout << "Waiting patients (sorted): ";
        for (int k = 0; k < size_; ++k) cout << (k ? ", " : "") << tmp[k];
        cout << "\n";
        delete[] tmp;
    }
};

// ---------------------------------------------------------------------------
// Menu driven program
// ---------------------------------------------------------------------------
static void printMenu() {
    cout << "\n===== Emergency Ward Waiting Room =====\n"
            " 1. Register patient\n"
            " 2. Serve patient\n"
            " 3. Cancel all appointments\n"
            " 4. Can the doctor go home?\n"
            " 5. Show all waiting patients (sorted)\n"
            " 0. Exit\n"
            "Choice: ";
}

int main() {
    WaitingRoom room;
    string choice;

    while (true) {
        printMenu();
        if (!getline(cin, choice)) {  // end of input
            cout << "\n";
            break;
        }

        if (choice == "1") {
            cout << "Patient registered with ID " << room.RegisterPatient() << ".\n";
        } else if (choice == "2") {
            int id = room.ServePatient();
            if (id == 0) cout << "No patients are waiting.\n";
            else cout << "Now serving patient " << id << ".\n";
        } else if (choice == "3") {
            cout << room.CancelAll() << " appointment(s) cancelled. The doctor can go to lunch.\n";
        } else if (choice == "4") {
            if (room.CanDoctorGoHome()) cout << "Yes - nobody is waiting (true).\n";
            else cout << "No - patients are still waiting (false).\n";
        } else if (choice == "5") {
            room.ShowAllPatient();
        } else if (choice == "0") {
            cout << "Goodbye.\n";
            break;
        } else {
            cout << "Invalid choice, try again.\n";
        }
    }
    return 0;
}