// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "cubic_spline.hpp"
#include "core.hpp"

#include <algorithm>
#include <stdexcept>


template <class Real>
CubicSpline<Real>::CubicSpline(const Tabulated<Real> &func) : grid(func.x), splines(func.size() - 1) {
    size_t n = func.size();

    if (n < 2) {
        throw std::invalid_argument("CubicSpline::CubicSpline: n < 2");
    }

    for (size_t i = 0; i != n - 1; ++i) {
        splines[i].a = func.y[i + 1];
    }

    std::vector<Real> A(n - 2), B(n - 2), C(n - 2), F(n - 2);
    Real h_i, h_i1;

    for (size_t i = 1; i != n - 1; ++i) {
        h_i = grid[i] - grid[i - 1];
        h_i1 = grid[i + 1] - grid[i];
        A[i - 1] = h_i;
        C[i - 1] = 2.0 * (h_i + h_i1);
        B[i - 1] = h_i1;
        F[i - 1] = 6.0 * ((func.y[i + 1] - func.y[i]) / h_i1 - (func.y[i] - func.y[i - 1]) / h_i);
    }

    auto c = tridiagonal_thomas(A, C, B, F);

    for (size_t i = 0; i != n - 2; ++i) {
        splines[i].c = c[i];
    }

    for (ssize_t i = n - 2; i >= 0; --i) {
        h_i = grid[i + 1] - grid[i];
        splines[i].d = (splines[i].c - splines[i - 1].c) / h_i;
        splines[i].b = h_i * (2.0 * splines[i].c + splines[i - 1].c) / 6.0 + (func.y[i + 1] - func.y[i]) / h_i;
    }

    grid.erase(grid.begin());
}


template <class Real>
Real CubicSpline<Real>::operator()(Real x) const {
    typename std::vector<SplineTuple>::const_iterator spline_it;
    auto grid_it = std::upper_bound(grid.begin(), grid.end(), x);

    if (grid_it == grid.end()) {
        --grid_it;
    }
    spline_it = splines.begin() + (grid_it - grid.begin());

    Real dx = x - *grid_it;

    return spline_it->a + (spline_it->b + (spline_it->c / 2.0 + spline_it->d * dx / 6.0) * dx) * dx;
}


template <class Real>
Tabulated<Real> tabulate_spline(const Tabulated<Real> &func, Real a, Real b, size_t n) {
    return tabulate_linspace(CubicSpline<Real>(func), a, b, n);
}


template class CubicSpline<double>;
template Tabulated<double> tabulate_spline(const Tabulated<double> &, double, double, size_t);
