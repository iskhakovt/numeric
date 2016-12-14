// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include <stdexcept>


template <class Real>
std::pair<Tabulated<Real>, Tabulated<Real>> runge_kutta(
    const FunctionThreeArg<Real> &f, const FunctionThreeArg<Real> &g, Real y0, Real z0, Real T, size_t n)
{
    if (n < 2) {
        throw std::invalid_argument("runge_kutta: n < 2");
    }

    std::vector<Real> x(n), y(n), z(n);
    Real h = T / (n - 1), f1, f2, f3, f4, g1, g2, g3, g4;

    x[0] = 0.0;
    y[0] = y0;
    z[0] = z0;

    for (size_t i = 1; i != n; ++i) {
        f1 = h * f(x[i - 1], y[i - 1], z[i - 1]);
        g1 = h * g(x[i - 1], y[i - 1], z[i - 1]);

        f2 = h * f(x[i - 1] + h / 2.0, y[i - 1] + f1 / 2.0, z[i - 1] + g1 / 2.0);
        g2 = h * g(x[i - 1] + h / 2.0, y[i - 1] + f1 / 2.0, z[i - 1] + g1 / 2.0);

        f3 = h * f(x[i - 1] + h / 2.0, y[i - 1] + f2 / 2.0, z[i - 1] + g2 / 2.0);
        g3 = h * g(x[i - 1] + h / 2.0, y[i - 1] + f2 / 2.0, z[i - 1] + g2 / 2.0);

        f4 = h * f(x[i - 1] + h, y[i - 1] + f3, z[i - 1] + g3);
        g4 = h * g(x[i - 1] + h, y[i - 1] + f3, z[i - 1] + g3);

        x[i] = i * h;
        y[i] = y[i - 1] + (f1 + f2 * 2.0 + f3 * 2.0 + f4) / 6.0;
        z[i] = z[i - 1] + (g1 + g2 * 2.0 + g3 * 2.0 + g4) / 6.0;
    }

    return {Tabulated<Real>(x, y), Tabulated<Real>(x, z)};
}

template std::pair<Tabulated<double>, Tabulated<double>> runge_kutta(
const FunctionThreeArg<double> &, const FunctionThreeArg<double> &, double, double, double, size_t);
