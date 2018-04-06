#ifndef NU_MATPLOTLIB_HPP
#define NU_MATPLOTLIB_HPP

/**
 * Convenience wrapper-header for matplotlibcpp.h (https://github.com/lava/matplotlib-cpp)
 */

// NOTE: we include math.h to avoid errors (appearing on windows when compiling with gcc/clang using MSYS2)
// of type "no member named ::X in global namespace". These emerge from Python.h->pyport.h for some reason.
#include <math.h> // NOLINT
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#include <matplotlibcpp.h>
#pragma GCC diagnostic pop

#include <Range.hpp>

// alias long namespace name. Ple-e-ease, stop naming namespaces like that!
namespace mpl = matplotlibcpp;

namespace nya {

template <typename T, typename Fn>
void plot(Range<T> xRange, Fn function, const char* format) {
    // make xVals physical vector // todo avoid?
    std::vector<T> xVals (xRange.count());
    auto rangeIterator = xRange.begin();
    std::generate(xVals.begin(), xVals.end(), [&rangeIterator]() {
        return *rangeIterator++;
    });
    PyObject* xarray = mpl::get_array(xVals);

    // make yVals physical vector // todo avoid?
    std::vector<T> yVals (xRange.count());
    std::transform(xRange.begin(), xRange.end(), yVals.begin(), function);
    PyObject* yarray = mpl::get_array(yVals);

    PyObject* pystring = PyString_FromString(format);

    PyObject* plot_args = PyTuple_New(3);
    PyTuple_SetItem(plot_args, 0, xarray);
    PyTuple_SetItem(plot_args, 1, yarray);
    PyTuple_SetItem(plot_args, 2, pystring);

    PyObject* res = PyObject_CallObject(mpl::detail::_interpreter::get().s_python_function_plot, plot_args);

    Py_DECREF(plot_args);
    if (res) Py_DECREF(res);
};

} // nya

#endif //NU_MATPLOTLIB_HPP
