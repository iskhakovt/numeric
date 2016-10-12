// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_TABULATED_FUNCTION_HPP
#define NUMERIC_CORE_TABULATED_FUNCTION_HPP


#include <stdexcept>
#include <vector>


struct Tabulated {
    std::vector<double> x, y;

    Tabulated() {}
    Tabulated(const std::vector<double> &x, const std::vector<double> &y) : x(x), y(y) {
        if (x.size() != y.size()) {
            throw std::invalid_argument("tabluated different sizes");
        }
    }

    double get_size() const { return x.size(); }

    double get_value(double) const { return 0.0; }
};


#endif //NUMERIC_CORE_TABULATED_FUNCTION_HPP
