#ifndef SRC_HPP
#define SRC_HPP
#include <iostream>
#include <cstddef>

struct Permutation { // 置换
    int* mapping; // i -> mapping[i]
    size_t size;

    explicit Permutation(size_t size) : mapping(nullptr), size(size) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = static_cast<int>(i);
    }

    Permutation(const int* src, size_t size) : mapping(nullptr), size(size) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = src[i];
    }

    Permutation(const Permutation& other) : mapping(nullptr), size(other.size) {
        mapping = new int[size];
        for (size_t i = 0; i < size; ++i) mapping[i] = other.mapping[i];
    }
    Permutation(Permutation&& other) noexcept : mapping(other.mapping), size(other.size) {
        other.mapping = nullptr;
        other.size = 0;
    }
    Permutation& operator=(const Permutation& other) {
        if (this == &other) return *this;
        int* nm = new int[other.size];
        for (size_t i = 0; i < other.size; ++i) nm[i] = other.mapping[i];
        delete[] mapping;
        mapping = nm;
        size = other.size;
        return *this;
    }
    Permutation& operator=(Permutation&& other) noexcept {
        if (this == &other) return *this;
        delete[] mapping;
        mapping = other.mapping;
        size = other.size;
        other.mapping = nullptr;
        other.size = 0;
        return *this;
    }

    ~Permutation() {
        delete[] mapping;
        mapping = nullptr;
        size = 0;
    }

    void apply(int* permutation) const {
        int* tmp = new int[size];
        for (size_t i = 0; i < size; ++i) tmp[i] = permutation[mapping[i]];
        for (size_t i = 0; i < size; ++i) permutation[i] = tmp[i];
        delete[] tmp;
    }

    Permutation operator*(const Permutation& other) const {
        Permutation res(size);
        for (size_t i = 0; i < size; ++i) res.mapping[i] = other.mapping[mapping[i]];
        return res;
    }

    Permutation inverse() const {
        Permutation inv(size);
        for (size_t j = 0; j < size; ++j) inv.mapping[mapping[j]] = static_cast<int>(j);
        return inv;
    }

    friend std::ostream& operator<<(std::ostream& os, const Permutation& p) {
        os << '[';
        for (size_t i = 0; i < p.size; ++i) {
            os << p.mapping[i];
            if (i < p.size - 1) os << ' ';
        }
        os << ']';
        return os;
    }
};

struct Transposition { // 对换
    int a, b;

    Transposition() : a(0), b(0) {}
    Transposition(int a_, int b_) : a(a_), b(b_) {}

    void apply(int* permutation, size_t /*size*/) const {
        if (a == b) return;
        int t = permutation[a];
        permutation[a] = permutation[b];
        permutation[b] = t;
    }

    friend std::ostream& operator<<(std::ostream& os, const Transposition& t) {
        os << '{' << t.a << ' ' << t.b << '}';
        return os;
    }

    Permutation toPermutation(size_t size) const {
        Permutation res(size);
        if (a != b) {
            int t = res.mapping[a];
            res.mapping[a] = res.mapping[b];
            res.mapping[b] = t;
        }
        return res;
    }
};

struct Cycle { // 轮换
    int* elements;
    size_t size;

    Cycle(const int* elems, size_t k) : elements(nullptr), size(k) {
        elements = new int[size];
        for (size_t i = 0; i < size; ++i) elements[i] = elems[i];
    }

    Cycle(const Cycle& other) : elements(nullptr), size(other.size) {
        elements = new int[size];
        for (size_t i = 0; i < size; ++i) elements[i] = other.elements[i];
    }
    Cycle(Cycle&& other) noexcept : elements(other.elements), size(other.size) {
        other.elements = nullptr;
        other.size = 0;
    }
    Cycle& operator=(const Cycle& other) {
        if (this == &other) return *this;
        int* ne = new int[other.size];
        for (size_t i = 0; i < other.size; ++i) ne[i] = other.elements[i];
        delete[] elements;
        elements = ne;
        size = other.size;
        return *this;
    }
    Cycle& operator=(Cycle&& other) noexcept {
        if (this == &other) return *this;
        delete[] elements;
        elements = other.elements;
        size = other.size;
        other.elements = nullptr;
        other.size = 0;
        return *this;
    }

    ~Cycle() {
        delete[] elements;
        elements = nullptr;
        size = 0;
    }

    void apply(int* permutation, size_t n) const {
        if (size == 0 || size == 1) return;
        int* tmp = new int[n];
        for (size_t i = 0; i < n; ++i) tmp[i] = permutation[i];
        for (size_t j = 0; j < size; ++j) {
            size_t from = static_cast<size_t>(elements[(j + 1) % size]);
            size_t to = static_cast<size_t>(elements[j]);
            tmp[to] = permutation[from];
        }
        for (size_t i = 0; i < n; ++i) permutation[i] = tmp[i];
        delete[] tmp;
    }

    friend std::ostream& operator<<(std::ostream& os, const Cycle& c) {
        os << '{';
        for (size_t i = 0; i < c.size; ++i) {
            os << c.elements[i];
            if (i < c.size - 1) os << ' ';
        }
        os << '}';
        return os;
    }

    Permutation toPermutation(size_t n) const {
        Permutation res(n);
        if (size == 0 || size == 1) return res;
        for (size_t j = 0; j < size; ++j) {
            int to = elements[j];
            int from = elements[(j + 1) % size];
            res.mapping[to] = from;
        }
        return res;
    }
};


#endif
