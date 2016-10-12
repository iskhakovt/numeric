// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

double integral(const Tabulated &func, double y) {
    std::cerr << "integral" << std::endl;

    return 0.0;
}

Tabulated tabulate_integral(const Tabulated &func) {
    std::cerr << "tabulate_integral" << std::endl;

    std::vector<double> x(GRID_SIZE), y(GRID_SIZE);

    for (size_t i = 0; i <= GRID_SIZE; ++i) {
        double xVal = 1.0 / GRID_SIZE * i;

        x[i] = xVal;
        y[i] = integral(func, xVal);
    }

    return Tabulated(x, y);
}
