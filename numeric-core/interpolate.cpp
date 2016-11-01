// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

template <class Real>
Polynomial<Real> interpolate(const Tabulated<Real> &func) {
    std::cerr << "interpolate" << std::endl;

    return {};
}


template Polynomial<double> interpolate(const Tabulated<double> &);
