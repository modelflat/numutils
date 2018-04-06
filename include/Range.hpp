#ifndef NUMUTILS_RANGE_HPP
#define NUMUTILS_RANGE_HPP

#include <iterator>

namespace nya {

template <int64_t value, int64_t power>
struct IntegralPow {
    static constexpr int64_t v = value * IntegralPow<value, power - 1>::v;
};

template <int64_t value>
struct IntegralPow<value, 0> {
    static constexpr int64_t v = 1;
};

/**
 * Range iterator implemented as RA iterator.
 */
template <typename T = double>
class RangeIterator : public std::iterator<std::random_access_iterator_tag, T, size_t, const T*, T> {
    size_t from_;
    T start_, step_;
public:
    RangeIterator(size_t from, T start, T step) : from_(from), start_(start), step_(step) {}
    // add
    RangeIterator& operator++() { ++from_; return *this; }
    const RangeIterator operator++(int) { auto it = *this; ++(*this); return it; }
    RangeIterator& operator+=(size_t n) { from_ += n; return *this; }
    RangeIterator operator+(size_t n) const { auto it = *this; it += n; return it; }
    // sub
    RangeIterator& operator--() { --from_; return *this; }
    const RangeIterator operator--(int) { auto it = *this; --(*this); return it; }
    RangeIterator& operator-=(size_t n) { from_ -= n; return *this; }
    RangeIterator operator-(size_t n) const { auto it = *this; it -= n; return it; }
    size_t operator-(RangeIterator other) const { return this->from_ - other.from_; }

    // compare
    bool operator==(RangeIterator other) const { return from_ == other.from_; }
    bool operator!=(RangeIterator other) const { return !(*this == other);}
    bool operator<(RangeIterator other) const { return from_ < other.from_; }
    bool operator>(RangeIterator other) const { return from_ > other.from_; }
    bool operator<=(RangeIterator other) const { return !(*this > other); }
    bool operator>=(RangeIterator other) const { return !(*this < other); }

    // access
    T operator[](size_t n) const { return start_ + (from_ + n) * step_; }
    T operator*() const { return start_ + from_ * step_; }
};

template <typename T = double>
class Range {
    T start_;
    size_t count_;
    T step_;

public:
    template <typename Size, typename = std::enable_if_t<std::is_integral_v<Size>>>
    Range(T start, Size count, T step) : start_(start), count_(count), step_(step) {}

    inline size_t startIdx() const noexcept {
        return static_cast<size_t>( start_ / step_ );
    }

    inline size_t stopIdx() const noexcept {
        return startIdx() + count_ - 1;
    }

    inline size_t count() const noexcept {
        return count_;
    }

    inline T step() const noexcept {
        return step_;
    }

    inline auto begin() const noexcept {
        return RangeIterator<T> { 0, start_, step_ };
    }

    inline auto end() const noexcept {
        return RangeIterator<T> { count_, start_, step_ };
    }

};


template <size_t stepOrder = 6, typename T = double, ssize_t __v = IntegralPow<10, stepOrder>::v>
auto discreteRange(T from, T to) {
    return Range { from,  __v, (to - from) / __v };
}

} // nya

#endif //NUMUTILS_RANGE_HPP
