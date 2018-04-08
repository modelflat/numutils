#ifndef NUMUTILS_NUMERICALUTILS_HPP
#define NUMUTILS_NUMERICALUTILS_HPP

#include <numeric>
#include <algorithm>
#include <functional>
#include <cmath>
#include <tuple>

#include "Surface.hpp"
#include "Range.hpp"
#include "PrecisionTraits.hpp"

#ifdef NUMERICALUTILS_DEBUG_OUTPUT
#include <iostream>
#include <iomanip>

template <typename T>
void printSurface(const char* cap, const Surface<T>& surf) {
    std::cout << cap << ": " << std::endl;
    for (size_t i = 0; i < surf.rowCount(); ++i) {
        std::cout << std::setprecision(6) << std::left << std::setw(10);
        for (size_t j = 0; j < surf.columnCount(); ++j) {
            std::cout << surf.at(i, j) << ' ';
        }
        std::cout << std::endl;
    }
}

template <typename T>
void printVector(const char* cap, const std::vector<T>& v) {
    std::cout << cap << ": ";
    std::cout << std::setprecision(6) << std::setw(10);
    std::for_each(v.begin(), v.end(), [](auto el) { std::cout << el << ' '; });
    std::cout << std::endl;
}

#define DEBUG_PRINT_SURFACE(caption, surface) \
    printSurface(#caption, surface)

#define DEBUG_PRINT_VECTOR(caption, vector) \
    printVector(#caption, vector)
#else
#define DEBUG_PRINT_SURFACE(caption, surface);
#define DEBUG_PRINT_VECTOR(caption, vector);
#endif

namespace nya {

template <typename F>
auto negate(F f) {
    return [=](auto... x) { return -f(x...); };
}

template <typename ... Fs>
auto sum(Fs ... f) {
    return [=](auto... x) { return (0.0 + ... + f(x...)); };
}

template <typename ... Fs>
auto product(Fs ... f) {
    return [=](auto... x) { return (1.0 * ... * f(x...)); };
}

template <
    template <typename> typename Stepper, size_t var = 0, typename T = double, typename F
>
auto integral(F f) {
    Stepper<T> stepper;
    return [=] (Range<T> D, auto... x0) {
        if constexpr (sizeof...(x0) == 0) {
            return std::accumulate(D.begin(), D.end(), 0.0,
                                   [stepper, f, D](T acc, T x) { return acc + stepper(f, D.step(), x); });
        } else {
            auto fBound = [f, x0...](auto x) mutable { // todo heavy lambda?
                auto _tX0 = std::forward_as_tuple(std::forward<decltype(x0)>(x0)...);
                std::get<var>(_tX0) = x;
                return f(x0...);
            };
            return std::accumulate(D.begin(), D.end(), 0.0,
                                   [stepper, fBound, D](T acc, T x) { return acc + stepper(fBound, D.step(), x); });
        }
    };
}

template <typename T>
struct Euler {
    template <typename F>
    T operator()(F f, T h, T x) const {
        return h * f(x);
    }
};

template <typename T>
struct RK4 {
    template <typename F>
    T operator()(F f, T h, T x) const {
        const auto k1 = f(x);
        const auto k2 = f(x + k1 * h / 2);
        const auto k3 = f(x + k2 * h / 2);
        const auto k4 = f(x + k3 * h);
        return h / 6.0 * (k1 + 2*k2 + 2*k3 + k4);
    }
};

template <
    template <typename> typename Stepper, size_t var = 0, typename T = double, typename ... Fs
>
auto innerProduct(Fs ... functions) {
    const auto pr = product(functions...);
    return integral<Stepper, var, T>([=](auto... x) { return pr( x... ); });
}

template <
    template <typename> typename DiffMethod, size_t order = 1, size_t var = 0, typename T = double, typename F
>
auto D(F f) {
    DiffMethod<T> method;
    return [=](auto... x) { return method.template compute<order, var>(f, x...); };
}

template <typename T>
struct LSFD1 {
    template <size_t order, size_t var, typename F, typename ...Args>
    auto compute(F f, Args... x) const { // not operator() to more intuitive template args
        const auto h = PrecisionTraits<T>::derivativePrecision(order);
        const auto base = f(x...);
        auto vars = std::forward_as_tuple(std::forward<decltype(x)>(x)...);
        std::get<var>(vars) -= h;
        const auto left = f(x...);
        auto result = ( base - left ) / h;
        return result;
    }
};

template <typename T>
struct LSFD2 {
    template <size_t order, size_t var, typename F, typename ...Args>
    auto compute(F f, Args... x) const {
        const auto h = PrecisionTraits<T>::derivativePrecision(order);
        auto vars = std::forward_as_tuple(std::forward<decltype(x)>(x)...);
        const auto base  = f(x...);
        std::get<var>(vars) -= h;
        const auto left  = f(x...);
        std::get<var>(vars) += 2.0 * h;
        const auto right = f(x...);
        return ( 3.0*base - 4.0*left + right ) / (2.0 * h);
    }
};

template <typename T>
struct RSFD1;
template <typename T>
struct RSFD2;
template <typename T>
struct CSFD2;
template <typename T>
struct CSFD4;


template <typename T, typename Alloc = std::allocator<T>>
using Vector = std::vector<T, Alloc>;

/**
 * Naive Gaussian elimination implementation
 */
template <typename T>
Vector<T> eliminate(Surface<T>&& A) {
    size_t N = A.rowCount();

    for (size_t i = 0; i < N; ++i) {
        // Search for maximum in this column
        T maxEl = std::abs( A.at(i, i) );
        size_t maxRow = i;
        for (size_t k = i + 1; k < N; ++k) {
            const T el = std::abs(A.at(k, i));
            if (el > maxEl) {
                maxEl = el; maxRow = k;
            }
        }

        if (maxRow != i) {
            // Swap maximum row with current row (column by column)
            for (size_t k = i; k < N + 1; ++k) {
                std::swap(A.at(maxRow, k), A.at(i, k));
            }
        }

        // eliminate rows below
        for (size_t k = i + 1; k < N; ++k) {
            T c = -A.at(k, i) / A.at(i, i);
            for (size_t j = i; j < N + 1; ++j) {
                if (i == j) {
                    A.at(k, j) = 0;
                } else {
                    A.at(k, j) += c * A.at(i, j);
                }
            }
        }
    }

    Vector<T> solution ( N );
    for (ssize_t i = N - 1; i >= 0; --i) {
        solution[i] = A.at(i, N) / A.at(i, i);
        for (ssize_t k = i-1; k >= 0; --k) {
            A.at(k, N) -= A.at(k, i) * solution[i];
        }
    }
    return solution;
}

template <typename T, typename F>
auto makeTrialFunction(std::vector<F> trials, std::vector<T> coefs) {
    return [=](T x) {
        T res = 0;
        for (size_t i = 0; i < trials.size(); ++i) {
            res += coefs[i]*trials[i](x);
        }
        return res;
    };
}

template <template <typename> typename Stepper, size_t var = 0, typename T = double, typename DiffOp, typename F>
auto galerkin(DiffOp LOp, std::vector<F> trials) {
    return [=](Range<T> range) {
        Surface<T> matrix (trials.size() - 1, trials.size());
        for (size_t row = 0; row < matrix.rowCount(); ++row) {
            auto phiK = trials[row];
            // compute free coef
            *(matrix.begin() + matrix.columnCount()*row + trials.size() - 1) =
                -innerProduct<Stepper, var, T>(LOp(trials[0]), phiK)(range);
            // compute other coefficents
            std::transform(trials.begin() + 1, trials.end(), matrix.begin() + matrix.columnCount() * row,
                               [phiK, range, LOp](auto phiJ) {
                               return innerProduct<Stepper, var, T>(LOp(phiJ), phiK)(range);
                           });
        }
        DEBUG_PRINT_SURFACE("Galerkin method -- out matrix", matrix);
        auto trialCoefs = eliminate(std::move(matrix) );
        trialCoefs.insert(trialCoefs.begin(), 1.0); // todo optimize?
        DEBUG_PRINT_VECTOR("Trial coefs", trialCoefs);
        return makeTrialFunction<T, F>(trials, trialCoefs);
    };
}

template <typename T = double>
auto simplePolynomials(size_t maxOrder) {
    std::vector<std::function<T(T)>> fns;
    fns.reserve((maxOrder + 1));
    for (size_t i = 0; i <= maxOrder; ++i) {
        fns.emplace_back( [i](auto x) { return std::pow(x, i); } );
    }
    return fns;
}

} // nya

#endif //NUMUTILS_NUMERICALUTILS_HPP
