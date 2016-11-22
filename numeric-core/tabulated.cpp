// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "tabulated.hpp"
#include "core.hpp"


template <class Real>
Tabulated<Real>::Tabulated(const std::vector<Real> &x, const std::vector<Real> &y) : x(x), y(y) {
    if (x.size() != y.size()) {
        throw std::invalid_argument("tabluated different sizes");
    }

    for (size_t i = 1; i != y.size(); ++i) {
        if (x[i] < x[i - 1] || equal(x[i], x[i - 1])) {
            throw std::invalid_argument("grid must be sorted and unique");
        }
    }
}


template class Tabulated<double>;
