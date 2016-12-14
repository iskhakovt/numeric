// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MODEL_RESULTS_HPP
#define NUMERIC_LOGIC_MODEL_RESULTS_HPP


#include "tabulated.hpp"

#include <stdexcept>


template <class Real>
struct ModelResults {
    Tabulated<Real> x;
    Tabulated<Real> y;
    Real C1;
    Real C2;
    Real F;

    ModelResults() {}
    ModelResults(
        const Tabulated<Real> &x,
        const Tabulated<Real> &y,
        Real C1,
        Real C2,
        Real F
    ) : x(x), y(y), C1(C1), C2(C2), F(F) {
        if (x.x != y.x) {
            throw std::invalid_argument("ModelResults::ModelResults: x, y different grids");
        }
    }

    bool operator<(const ModelResults &result) const {
        return F < result.F;
    }
};


#endif //NUMERIC_LOGIC_MODEL_RESULTS_HPP
