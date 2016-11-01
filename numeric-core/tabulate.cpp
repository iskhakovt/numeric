// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include <algorithm>
#include <cmath>


#include <iostream>

template <class Real>
Tabulated<Real> tabulate(const Function<Real> &func, const std::vector<Real> &grid) {
    std::cout << "tabulate" << std::endl;

    std::vector<Real> func_val(grid.size());

    for (size_t i = 0; i != grid.size(); ++i) {
        func_val[i] = func(grid[i]);
    }

    return Tabulated<Real>(grid, func_val);
}


template <class Real>
std::vector<Real> chebyshev_roots(size_t n, Real a, Real b) {
    std::vector<Real> roots(n);

    for (size_t i = 0; i != n; ++i) {
        roots[i] = 0.5 * (a + b) + 0.5 * (b - a) * cos((2.0 * i + 1.0) / (2.0 * n) * M_PI);
    }

    std::sort(roots.begin(), roots.end());
    return roots;
}


template <class Real>
Tabulated<Real> tabulate_chebyshev(const Function<Real> &func, size_t n) {
    std::cout << "tabulate_chebyshev" << std::endl;

    return tabulate(func, chebyshev_roots(n, -1.0, 1.0));
}


template Tabulated<double> tabulate(const Function<double> &, const std::vector<double> &);
template std::vector<double> chebyshev_roots(size_t, double, double);
template Tabulated<double> tabulate_chebyshev(const Function<double> &, size_t);
