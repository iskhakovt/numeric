// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

template <class Real>
Tabulated<Real> differential_equation(const ModelArguments<Real> &args, Real beta) {
    std::cerr << "differential_equation" << std::endl;

    return {};
}

template <class Real>
Tabulated<Real> differential_equation(const Tabulated<Real> &func, Real x0, Real y0, Real T) {
    std::cerr << "differential_equation" << std::endl;

    std::vector<Real> x, y;

    for (size_t i = 0; i != GRID_SIZE; ++i) {
        Real t_prev = T / (i - 1);
        Real t = T / i;

        if (i == 0) {
            x[0] = x0;
            y[0] = y0;
        } else {
            x[i] = x[i - 1] + (t - t_prev) * x[i - 1];
            y[i] = y[i - 1] + (t - t_prev) * func(x[i - 1]);
        }
    }

    return Tabulated<Real>(x, y);
}


template Tabulated<double> differential_equation(const ModelArguments<double> &, double);
template Tabulated<double> differential_equation(const Tabulated<double> &, double, double, double);
