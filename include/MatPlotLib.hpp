#ifndef NUMERICAL4THCOURSE_MATPLOTLIB_HPP
#define NUMERICAL4THCOURSE_MATPLOTLIB_HPP

/**
 * Convenience wrapper-header for matplotlibcpp.h (https://github.com/lava/matplotlib-cpp)
 */

// NOTE: we include math.h to avoid errors (appearing on windows when compiling with gcc/clang using MSYS2)
// of type "no member named ::X in global namespace". These emerge from Python.h->pyport.h for some reason.
#include <math.h> // NOLINT
#include <matplotlibcpp.h>

// alias long namespace name. Ple-e-ease, stop naming namespaces like that!
namespace mpl = matplotlibcpp;

#endif //NUMERICAL4THCOURSE_MATPLOTLIB_HPP
