// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

Tabulated tabulate(std::function<double(double)> func, const std::vector<double> &grid) {
    std::cout << "tabulate" << std::endl;

    std::vector<double> func_val(grid.size());

    for (size_t i = 0; i != grid.size(); ++i) {
        func_val[i] = func(grid[i]);
    }

    return {grid, func_val};
}

Tabulated tabulate_chebyshev(std::function<double(double)> func, size_t n) {
    std::cout << "tabulate_chebyshev" << std::endl;

    std::vector<double> y(n);
    for (size_t i = 0; i != n; ++i) {
    	y[i] = static_cast<double>(i);
    }

    return tabulate(func, y);
}
