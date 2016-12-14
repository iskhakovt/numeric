// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include "gauss-kronrod.hpp"

#include <stdexcept>


template <class Real>
Real integral(const Function<Real> &func, Real a, Real b) {
    return integral_gauss_kronrod(func, a, b, 61);
}


template <class Real>
Real integral_gauss_kronrod(const Function<Real> &func, Real a, Real b, size_t n) {
    if (n < 3) throw std::invalid_argument("integral_gauss_kronrod: too low grid size");
    if (n % 4 == 1 || n % 4 == 2) n -= 2;

    GaussKronrod<Real> GK((n - 1) / 2);

    Real result, rb, rc, rd;
    GK.qk(func, a, b, result, rb, rc, rd);

    return result;
}


template <class Real>
Real integral_simpson(const Function<Real> &func, Real a, Real b, size_t n) {
    if (n < 3) throw std::invalid_argument("integral_simpson: too low grid size");

    Real result = 0.0;

    n = (n + 1) / 2;
    std::vector<Real> x = linspace(a, b, n);

    Real lastVal = func(x[0]);

    for (size_t i = 1; i != n; ++i) {
        Real nextVal = func(x[i]);
        result += (x[i] - x[i - 1]) / 6.0 * (lastVal + nextVal + 4.0 * func((x[i] + x[i - 1]) / 2.0));
        lastVal = nextVal;
    }

    return result;
}


template <class Real>
Tabulated<Real> tabulate_integral(const Function<Real> &func) {
    std::vector<Real> x = linspace(0.0, 1.0, GRID_SIZE);
    std::vector<Real> y(x.size());

    for (size_t i = 0; i != x.size(); ++i) {
        y[i] = integral(func, x[i], 1.0);
    }

    return Tabulated<Real>(x, y);
}


template <class Real>
Tabulated<Real> tabulate_integral_tab(const Tabulated<Real> &func) {
    std::vector<Real> x = func.x, y(func.size(), 0.0);

    for (size_t i = 0; i != x.size(); ++i) {
        for (size_t j = i + 1; j != x.size(); ++j) {
            y[i] += (x[j] - x[j - 1]) * (func.y[j] + func.y[j - 1]) / 2.0;
        }
    }

    return Tabulated<Real>(x, y);
}


template double integral(const Function<double> &, double, double);
template double integral_gauss_kronrod(const Function<double> &, double, double, size_t);
template double integral_simpson(const Function<double> &, double, double, size_t);
template Tabulated<double> tabulate_integral(const Function<double> &);
template Tabulated<double> tabulate_integral_tab(const Tabulated<double> &);
