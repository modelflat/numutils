#ifndef NUMUTILS_PRECISIONTRAITS_HPP
#define NUMUTILS_PRECISIONTRAITS_HPP

#include <cfenv>

namespace nya {

template< typename T >
struct PrecisionTraitsBase {
    constexpr static T derivativePrecision(size_t order);
    constexpr static T derivativeError();
};

template< typename T >
struct PrecisionTraits : PrecisionTraitsBase<T> {
};

template<>
struct PrecisionTraits<double> {
    constexpr static double derivativePrecision(size_t order) { return 1e-10; }

    constexpr static double derivativeError() { return 1e-6; }
};

template<>
struct PrecisionTraits<float> {
    constexpr static float derivativePrecision(size_t order) { return 1e-6f; }

    constexpr static float derivativeError() { return 1e-3; }
};

}

#endif //NUMUTILS_PRECISIONTRAITS_HPP
