// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include "gauss-kronrod.hpp"


#include <iostream>

template <class Real>
Real integral(const Function<Real> &func, Real a, Real b) {
    std::cerr << "integral" << std::endl;

    return integral_gauss_kronrod(func, a, b, 61);
}


template <class Real>

Real integral_gauss_kronrod(const Function<Real> &func, Real a, Real b, size_t n) {
    std::cerr << "integral Gauss-Kronrod" << std::endl;

    if (n < 3) throw std::invalid_argument("Gauss-Kronrod too low grid size");
    if (n % 4 == 1 || n % 4 == 2) n -= 2;

    GaussKronrod<Real> GK((n - 1) / 2);

    Real result, rb, rc, rd;
    GK.qk(func, a, b, result, rb, rc, rd);

    return result;
}


template <class Real>
Real integral_simpson(const Function<Real> &func, Real a, Real b, size_t n) {
    std::cerr << "integral simpson" << std::endl;

    if (n < 3) throw std::invalid_argument("Simpson too low grid size");

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
    std::cerr << "tabulate_integral" << std::endl;

    std::vector<Real> x(GRID_SIZE), y(GRID_SIZE);

    for (size_t i = 0; i <= GRID_SIZE; ++i) {
        Real xVal = 1.0 / GRID_SIZE * i;

        x[i] = xVal;
        y[i] = integral(func, xVal, 1.0);
    }

    return Tabulated<Real>(x, y);
}


template double integral(const Function<double> &, double, double);
template double integral_gauss_kronrod(const Function<double> &, double, double, size_t);
template double integral_simpson(const Function<double> &, double, double, size_t);
template Tabulated<double> tabulate_integral(const Function<double> &);
