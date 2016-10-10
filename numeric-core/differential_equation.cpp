// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <vector>
#include <iostream>

Tabulated differential_equation(const ModelArguments &args, double beta) {
    std::cerr << "differential_equation" << std::endl;

    return {};
}

Tabulated differential_equation(const Tabulated &func, double x0, double y0, double T) {
    std::cerr << "differential_equation" << std::endl;

    std::vector<double> x, y;

    for (size_t i = 0; i != GRID_SIZE; ++i) {
        double t_prev = T / (i - 1);
        double t = T / i;

        if (i == 0) {
            x[0] = x0;
            y[0] = y0;
        } else {
            x[i] = x[i - 1] + (t - t_prev) * x[i - 1];
            y[i] = y[i - 1] + (t - t_prev) * func.get_value(x[i - 1]);
        }
    }

    return Tabulated(x, y);
}
