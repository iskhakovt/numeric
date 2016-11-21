// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


template <class Real>
Polynomial<Real> interpolate(const Tabulated<Real> &func) {
    return {};
}


template Polynomial<double> interpolate(const Tabulated<double> &);
