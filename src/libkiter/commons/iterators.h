//
// Created by toky on 4/10/22.
//

#ifndef KITER_ITERATORS_H
#define KITER_ITERATORS_H

#include <optional>

// The range_t structure contains begin and end,
// the only required function to be used in a foreach.
template<class It>
struct range_t {
    It b, e;
    It begin() const { return b; }
    It end() const { return e; }
};

// This iterator is used to go through a NaiveVertexData vector using the NaiveVertexRef object
template<class T>
struct incremental_iterator {

    std::optional<T> current;
    size_t cnt;

    T operator*() const {
        return *current;
    }
    // We increment until we reach last, then return nullptr.
    incremental_iterator& operator++() {
        if (cnt == 0)  {
            current = {};
        }
        if (current) {
            current.value()++ ;
            cnt--;
        }
        return *this;
    }

    incremental_iterator operator++(int) {
        auto r = *this;
        ++(*this);
        return r;
    }

    // iterators are only equivalent when both undefined
    friend bool operator==(incremental_iterator const& lhs, incremental_iterator const& rhs ) {
        return !lhs.current && !rhs.current;
    }
    friend bool operator!=(incremental_iterator const& lhs, incremental_iterator const& rhs ) {
        return !(lhs==rhs);
    }

    // We decrement the counter, if counter is zero, empty list, current = {}
    incremental_iterator(std::optional<T> start, size_t counter ) {
        current = start;
        cnt     = counter - 1;

        if (counter == 0)  {
            current = {};
        }
    }


    // default all special member functions:
    incremental_iterator(incremental_iterator && ) =default;
    incremental_iterator(incremental_iterator const& ) =default;
    incremental_iterator& operator=(incremental_iterator && ) =default;
    incremental_iterator& operator=(incremental_iterator const& ) =default;
    incremental_iterator() =default;
};

#endif //KITER_ITERATORS_H
