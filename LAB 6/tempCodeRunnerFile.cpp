
#include <iostream>
#include <string>
using namespace std;
class WaitingRoom {
    int* data_;   
    int cap_;
    int head_;    
    int size_;     
    int nextId_; 
    void grow() {
        int* fresh = new int[cap_ * 2];
        for (int k = 0; k < size_; ++k) fresh[k] = data_[(head_ + k) % cap_];
        delete[] data_;
        data_ = fresh;
        head_ = 0;
        cap_ *= 2;}
    static void insertionSort(int* a, int n) {
        for (int i = 1; i < n; ++i) {
            int key = a[i];
            int j = i - 1;
            while (j >= 0 && a[j] > key) {
                a[j + 1] = a[j];
                --j;
            }
            a[j + 1] = key;}}
public:
    explicit WaitingRoom(int capacity = 5)
        : data_(new int[capacity]), cap_(capacity), head_(0), size_(0), nextId_(1) {}
    ~WaitingRoom() { delete[] data_; }
    WaitingRoom(const WaitingRoom&) = delete;
    WaitingRoom& operator=(const WaitingRoom&) = delete;
    int RegisterPatient() {
        if (size_ == cap_) grow();
        int id = nextId_++;
        data_[(head_ + size_) % cap_] = id;
        ++size_;
        return id;}
    int ServePatient() {
        if (size_ == 0) return 0;
        int id = data_[head_];
        head_ = (head_ + 1) % cap_;
        --size_;
        return id;}
    int CancelAll() {
        int cancelled = size_;
        head_ = 0;
        size_ = 0;
        return cancelled;}
    bool CanDoctorGoHome() const { return size_ == 0; }
    void ShowAllPatient() const {
        if (size_ == 0) {
            cout << "No patients are waiting.\n";
            return;}
        int* tmp = new int[size_];
        for (int k = 0; k < size_; ++k) tmp[k] = data_[(head_ + k) % cap_];
        insertionSort(tmp, size_);
        cout << "Waiting patients (sorted): ";
        for (int k = 0; k < size_; ++k) cout << (k ? ", " : "") << tmp[k];
        cout << "\n";
        delete[] tmp;
    }
};
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
            break;}
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
        }}
    return 0;}