// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_TABULATED_HPP
#define NUMERIC_CORE_TABULATED_HPP


#include "function.hpp"

#include <stdexcept>
#include <vector>


template <class Real>
struct Tabulated : public Function<Real> {
    std::vector<Real> x, y;

    Tabulated() {}
    Tabulated(const std::vector<Real> &x, const std::vector<Real> &y) : x(x), y(y) {
        if (x.size() != y.size()) {
            throw std::invalid_argument("tabluated different sizes");
        }
    }

    Real get_size() const { return x.size(); }

    Real operator()(Real) const override { return 0.0; }
};


#endif //NUMERIC_CORE_TABULATED_HPP
