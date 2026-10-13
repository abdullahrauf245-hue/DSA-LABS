/*
 * CS-250 Data Structures and Algorithms - Lab 6
 * Task 3: Print Server Request Handling System
 *
 * A circular queue (array based) with dynamic resizing, used to simulate a
 * single printer serving print requests.
 *
 * Resizing policy
 *   GROW   : enqueue on a full queue doubles the capacity.
 *   SHRINK : after a dequeue, if size <= capacity/4 and capacity > MIN_CAPACITY
 *            the capacity is halved (never below MIN_CAPACITY).
 *   resize(new_capacity) copies the items in logical order (front first) into a
 *   new array and resets head to 0, so no request is lost or reordered.
 *
 * Compile : g++ -std=c++17 -Wall -Wextra task3_print_server.cpp -o task3
 * Run     : ./task3
 */

#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

const int MIN_CAPACITY = 4;

// One print job: id, arrival time and processing time (in time units).
struct PrintRequest {
    int id;
    int arrivalTime;
    int processingTime;

    PrintRequest() : id(-1), arrivalTime(0), processingTime(0) {}  // id -1 = empty slot
    PrintRequest(int i, int a, int p) : id(i), arrivalTime(a), processingTime(p) {}
};

// ---------------------------------------------------------------------------
// Circular queue with dynamic resizing
// ---------------------------------------------------------------------------
class CircularQueue {
    PrintRequest* data_;
    int cap_;
    int head_;  // index of the front element
    int size_;  // number of stored elements

    int tail() const { return (head_ + size_) % cap_; }  // next write position

public:
    bool verbose;  // print a message whenever the queue is resized

    explicit CircularQueue(int capacity = MIN_CAPACITY)
        : data_(nullptr), cap_(capacity), head_(0), size_(0), verbose(true) {
        if (capacity < 1) throw invalid_argument("capacity must be at least 1");
        data_ = new PrintRequest[capacity];
    }
    ~CircularQueue() { delete[] data_; }
    CircularQueue(const CircularQueue&) = delete;
    CircularQueue& operator=(const CircularQueue&) = delete;

    bool isEmpty() const { return size_ == 0; }
    bool isFull() const { return size_ == cap_; }
    int size() const { return size_; }
    int capacity() const { return cap_; }

    // Insert x at the rear. Doubles the capacity first if the queue is full.
    void enqueue(const PrintRequest& x) {
        if (isFull()) resize(cap_ * 2);
        data_[tail()] = x;
        ++size_;
    }

    // Remove and return the front element. Shrinks if the queue gets sparse.
    PrintRequest dequeue() {
        if (isEmpty()) throw underflow_error("dequeue from empty queue");
        PrintRequest x = data_[head_];
        data_[head_] = PrintRequest();  // mark slot empty
        head_ = (head_ + 1) % cap_;
        --size_;
        if (cap_ > MIN_CAPACITY && size_ <= cap_ / 4) {
            int target = cap_ / 2;
            resize(target < MIN_CAPACITY ? MIN_CAPACITY : target);
        }
        return x;
    }

    // Return (without removing) the front element.
    const PrintRequest& front() const {
        if (isEmpty()) throw underflow_error("front of empty queue");
        return data_[head_];
    }

    // Move all elements into a new array of newCapacity, keeping their order.
    void resize(int newCapacity) {
        if (newCapacity < size_) throw invalid_argument("new capacity smaller than current size");
        PrintRequest* fresh = new PrintRequest[newCapacity];
        for (int k = 0; k < size_; ++k) fresh[k] = data_[(head_ + k) % cap_];
        if (verbose)
            cout << "      >> resize (" << (newCapacity > cap_ ? "GROW" : "SHRINK")
                 << "): capacity " << cap_ << " -> " << newCapacity << ", " << size_
                 << " request(s) copied\n";
        delete[] data_;
        data_ = fresh;
        head_ = 0;
        cap_ = newCapacity;
    }

    // Print the raw array, head/tail indices and the logical order.
    void show() const {
        cout << "      array : [ ";
        for (int i = 0; i < cap_; ++i) {
            if (i > 0) cout << " | ";
            if (data_[i].id < 0) cout << "_";
            else cout << "P" << data_[i].id;
        }
        cout << " ]\n      head=" << head_ << " tail=" << tail() << " size=" << size_
             << " cap=" << cap_ << "  order(front->rear): [";
        for (int k = 0; k < size_; ++k) {
            if (k > 0) cout << ", ";
            cout << "P" << data_[(head_ + k) % cap_].id;
        }
        cout << "]\n";
    }
};

// ---------------------------------------------------------------------------
// Simulation
// ---------------------------------------------------------------------------
/*
 * One-printer simulation.
 *   - All requests with arrivalTime <= clock are enqueued (in arrival order).
 *   - If the queue is empty the clock jumps to the next arrival.
 *   - Otherwise the front request is dequeued, starts printing at `clock`
 *     and the clock advances by its processing time.
 * waiting time of a request = start time - arrival time.
 * Returns the average waiting time.
 */
double simulate(const PrintRequest* requests, int n, int initialCapacity = MIN_CAPACITY) {
    // stable insertion sort of a copy by arrival time
    PrintRequest* pending = new PrintRequest[n];
    for (int i = 0; i < n; ++i) pending[i] = requests[i];
    for (int i = 1; i < n; ++i) {
        PrintRequest key = pending[i];
        int j = i - 1;
        while (j >= 0 && pending[j].arrivalTime > key.arrivalTime) {
            pending[j + 1] = pending[j];
            --j;
        }
        pending[j + 1] = key;
    }

    CircularQueue q(initialCapacity);
    int* start = new int[n];
    int* finish = new int[n];
    int* wait = new int[n];
    PrintRequest* served = new PrintRequest[n];
    int servedCount = 0, next = 0, clock = 0;

    while (next < n || !q.isEmpty()) {
        while (next < n && pending[next].arrivalTime <= clock) {
            const PrintRequest& r = pending[next];
            cout << "[t=" << clock << "] ENQUEUE P" << r.id << " (arrival=" << r.arrivalTime
                 << ", proc=" << r.processingTime << ")\n";
            q.enqueue(r);
            q.show();
            ++next;
        }
        if (q.isEmpty()) {
            clock = pending[next].arrivalTime;  // printer idle until next arrival
            continue;
        }
        PrintRequest r = q.dequeue();
        served[servedCount] = r;
        start[servedCount] = clock;
        wait[servedCount] = clock - r.arrivalTime;
        clock += r.processingTime;
        finish[servedCount] = clock;
        cout << "[t=" << start[servedCount] << "] DEQUEUE P" << r.id << " -> printing until t="
             << clock << " (waited " << wait[servedCount] << ")\n";
        q.show();
        ++servedCount;
    }

    // ---- report ----
    cout << "\n" << string(60, '=') << "\n"
         << left << setw(6) << "ID" << setw(10) << "Arrival" << setw(8) << "Proc" << setw(8)
         << "Start" << setw(8) << "Finish" << setw(6) << "Wait" << "\n"
         << string(60, '-') << "\n";
    long total = 0;
    for (int i = 0; i < servedCount; ++i) {
        cout << left << setw(6) << ("P" + to_string(served[i].id)) << setw(10)
             << served[i].arrivalTime << setw(8) << served[i].processingTime << setw(8)
             << start[i] << setw(8) << finish[i] << setw(6) << wait[i] << "\n";
        total += wait[i];
    }
    double avg = servedCount ? static_cast<double>(total) / servedCount : 0.0;
    cout << string(60, '-') << "\nRequests processed   : " << servedCount
         << "\nAverage waiting time : " << fixed << setprecision(2) << avg << "\n"
         << string(60, '=') << "\n";

    delete[] pending;
    delete[] start;
    delete[] finish;
    delete[] wait;
    delete[] served;
    return avg;
}

// ---------------------------------------------------------------------------
// Tests
// ---------------------------------------------------------------------------
static int failures = 0;

static void check(bool cond, const string& what) {
    if (!cond) {
        ++failures;
        cout << "  FAIL: " << what << "\n";
    }
}

static void testQueueBasics() {
    cout << string(60, '=') << "\nQueue ADT tests\n" << string(60, '=') << "\n";
    CircularQueue q(4);
    q.verbose = false;
    check(q.isEmpty() && !q.isFull(), "new queue is empty");

    for (int i = 1; i <= 4; ++i) q.enqueue(PrintRequest(i, i, 1));
    check(q.isFull() && q.front().id == 1, "full after 4 enqueues, front is 1");

    // wrap-around: free two slots at the front, then add two at the rear
    check(q.dequeue().id == 1, "dequeue 1");
    check(q.dequeue().id == 2, "dequeue 2");
    q.enqueue(PrintRequest(5, 5, 1));
    q.enqueue(PrintRequest(6, 6, 1));
    check(q.isFull() && q.capacity() == 4, "wrapped and full, capacity still 4");

    // growing while the data wraps must keep the order intact
    q.enqueue(PrintRequest(7, 7, 1));
    check(q.capacity() == 8, "grew to 8");
    int expected[] = {3, 4, 5, 6, 7};
    for (int k = 0; k < 5; ++k) check(q.dequeue().id == expected[k], "order kept after grow");
    check(q.isEmpty(), "empty after draining");

    // shrinking never loses data
    CircularQueue q2(4);
    q2.verbose = false;
    for (int i = 0; i < 20; ++i) q2.enqueue(PrintRequest(i, i, 1));
    for (int i = 0; i < 18; ++i) check(q2.dequeue().id == i, "FIFO order through shrink");
    check(q2.capacity() < 32, "capacity shrank");
    check(q2.dequeue().id == 18 && q2.dequeue().id == 19 && q2.isEmpty(), "last two intact");

    bool threw = false;
    try {
        q2.dequeue();
    } catch (const underflow_error&) {
        threw = true;
    }
    check(threw, "dequeue on empty throws");

    cout << (failures == 0 ? "All queue ADT tests passed.\n\n" : "Queue ADT tests FAILED.\n\n");
}

int main() {
    testQueueBasics();

    cout << string(60, '=') << "\nPrint server simulation\n" << string(60, '=') << "\n";
    // A burst early on forces the queue to grow; draining it forces a shrink.
    const PrintRequest data[] = {
        PrintRequest(1, 0, 5),  PrintRequest(2, 1, 4),  PrintRequest(3, 2, 6),
        PrintRequest(4, 3, 3),  PrintRequest(5, 4, 5),  PrintRequest(6, 5, 2),
        PrintRequest(7, 6, 4),  PrintRequest(8, 7, 3),  PrintRequest(9, 30, 2),
        PrintRequest(10, 31, 2)};
    double avg = simulate(data, 10);
    check(avg > 10.09 && avg < 10.11, "average waiting time is 10.10");

    cout << "\n" << (failures == 0 ? "ALL TESTS PASSED" : "SOME TESTS FAILED") << "\n";
    return failures == 0 ? 0 : 1;
}