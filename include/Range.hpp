#ifndef NUMERICAL4THCOURSE_RANGE_HPP
#define NUMERICAL4THCOURSE_RANGE_HPP

#include <iterator>

template <int64_t value, int64_t power>
struct IntegralPow {
    static constexpr int64_t v = value * IntegralPow<value, power - 1>::v;
};

template <int64_t value>
struct IntegralPow<value, 0> {
    static constexpr int64_t v = 1;
};

template <typename T>
class RangeIterator : public std::iterator<std::random_access_iterator_tag, T, size_t, const T*, T> {
    size_t from;
    T start, step;
public:
    RangeIterator(size_t from, T start, T step) : from(from), start(start), step(step) {}
    // add
    RangeIterator& operator++() { ++from; return *this; }
    const RangeIterator operator++(int) { auto it = *this; ++(*this); return it; }
    RangeIterator& operator+=(size_t n) { from += n; return *this; }
    RangeIterator operator+(size_t n) const { auto it = *this; it += n; return it; }
    // sub
    RangeIterator& operator--() { --from; return *this; }
    const RangeIterator operator--(int) { auto it = *this; --(*this); return it; }
    RangeIterator& operator-=(size_t n) { from -= n; return *this; }
    RangeIterator operator-(size_t n) const { auto it = *this; it -= n; return it; }
    size_t operator-(RangeIterator other) const { return this->from - other.from; }

    // compare
    bool operator==(RangeIterator other) const { return from == other.from; }
    bool operator!=(RangeIterator other) const { return !(*this == other);}
    bool operator<(RangeIterator other) const { return from < other.from; }
    bool operator>(RangeIterator other) const { return from > other.from; }
    bool operator<=(RangeIterator other) const { return !(*this > other); }
    bool operator>=(RangeIterator other) const { return !(*this < other); }
    // access
    T operator[](size_t n) const { return start + (from + n) * step; }
    T operator*() const { return start + from * step; }
};

template <typename T = double>
struct Range {
    T start;
    size_t count_;
    T step;

    template <typename Size, typename = std::enable_if_t<std::is_integral_v<Size>>>
    Range(T start, Size count, T step) : start(start), count_(count), step(step) {}

    template <size_t stepOrder = 5, ssize_t __v = IntegralPow<10, stepOrder>::v>
    Range(T from, T to) : start(from), count_(__v), step((to - from) / __v) {}

    [[deprecated]]
    Range(T start, T stop, T step)
        : start(start), count_(static_cast<size_t>(stop / step) - static_cast<size_t>(start / step) + 1), step(step) {}

    inline size_t startIdx() const noexcept {
        return static_cast<size_t>( start / step );
    }

    inline size_t stopIdx() const noexcept {
        return startIdx() + count_ - 1;
    }

    inline size_t count() const noexcept {
        return count_;
    }

    auto begin() const {
        return RangeIterator<T> { 0, start, step };
    }

    auto end() const {
        return RangeIterator<T> { count_, start, step };
    }

    template <size_t stepOrder = 5>
    static auto integral(T from, T to) {
        return Range { from, to };
    }
};

#endif //NUMERICAL4THCOURSE_RANGE_HPP
