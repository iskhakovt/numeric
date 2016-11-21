// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include <algorithm>
#include <cmath>


template <class Real>
Tabulated<Real> tabulate(const Function<Real> &func, const std::vector<Real> &grid) {
    std::vector<Real> func_val(grid.size());

    for (size_t i = 0; i != grid.size(); ++i) {
        func_val[i] = func(grid[i]);
    }

    return Tabulated<Real>(grid, func_val);
}


template <class Real>
std::vector<Real> linspace(Real a, Real b, size_t n) {
    std::vector<Real> ret(n);

    for (size_t i = 0; i != n; ++i) {
        ret[i] = a + (b - a) / (n - 1) * i;
    }

    return ret;
}


template <class Real>
Tabulated<Real> tabulate_linspace(const Function<Real> &func, Real a, Real b, size_t n) {
    return tabulate(func, linspace(a, b, n));
}


template <class Real>
std::vector<Real> chebyshev_roots(Real a, Real b, size_t n) {
    std::vector<Real> roots(n);

    for (size_t i = 0; i != n; ++i) {
        roots[i] = 0.5 * (a + b) + 0.5 * (b - a) * cos((2.0 * i + 1.0) / (2.0 * n) * M_PI);
    }

    std::sort(roots.begin(), roots.end());
    return roots;
}


template <class Real>
Tabulated<Real> tabulate_chebyshev(const Function<Real> &func, Real a, Real b, size_t n) {
    return tabulate(func, chebyshev_roots(-1.0, 1.0, n));
}


template Tabulated<double> tabulate(const Function<double> &, const std::vector<double> &);
template std::vector<double> linspace(double, double, size_t);
template std::vector<double> chebyshev_roots(double, double, size_t);
template Tabulated<double> tabulate_linspace(const Function<double> &, double, double, size_t);
template Tabulated<double> tabulate_chebyshev(const Function<double> &, double, double, size_t);
