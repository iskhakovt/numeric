// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


Tabulated tabulate(std::function<double(double)> func, const std::vector<double> &grid) {
    std::vector<double> func_val(grid.size());

    for (size_t i = 0; i != grid.size(); ++i) {
        func_val[i] = func(grid[i]);
    }

    return {grid, func_val};
}