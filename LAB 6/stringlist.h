/*
 * CS-250 Data Structures and Algorithms - Lab 6
 * stringlist.h - minimal growable array of strings (no std::vector).
 *
 * Used for token lists in Task 1 and for error messages in Task 2.
 * Follows the "rule of three": copy constructor, copy assignment and
 * destructor, so it is safe to copy and to return by value.
 */

#ifndef STRINGLIST_H
#define STRINGLIST_H

#include <string>

class StringList {
    std::string* a_;
    int n_, cap_;

    void grow() {
        std::string* b = new std::string[cap_ * 2];
        for (int i = 0; i < n_; ++i) b[i] = a_[i];
        delete[] a_;
        a_ = b;
        cap_ *= 2;
    }

public:
    StringList() : a_(new std::string[8]), n_(0), cap_(8) {}

    StringList(const StringList& o) : a_(new std::string[o.cap_]), n_(o.n_), cap_(o.cap_) {
        for (int i = 0; i < n_; ++i) a_[i] = o.a_[i];
    }

    StringList& operator=(const StringList& o) {
        if (this != &o) {
            delete[] a_;
            a_ = new std::string[o.cap_];
            n_ = o.n_;
            cap_ = o.cap_;
            for (int i = 0; i < n_; ++i) a_[i] = o.a_[i];
        }
        return *this;
    }

    ~StringList() { delete[] a_; }

    void add(const std::string& s) {
        if (n_ == cap_) grow();
        a_[n_++] = s;
    }

    int size() const { return n_; }
    const std::string& at(int i) const { return a_[i]; }

    void reverse() {
        for (int i = 0, j = n_ - 1; i < j; ++i, --j) {
            std::string t = a_[i];
            a_[i] = a_[j];
            a_[j] = t;
        }
    }
};

#endif  // STRINGLIST_H