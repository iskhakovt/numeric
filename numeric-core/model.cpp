// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

template <class Real>
Tabulated<Real> solve_model(const ModelArguments<Real> &args, Real beta) {
    std::cerr << "solve_model" << std::endl;

    return {};
}


template <class Real>
Real beta_search(const ModelArguments<Real> &args) {
    std::cerr << "beta_search" << std::endl;

    return 0.0;
}


template Tabulated<double> solve_model(const ModelArguments<double> &, double);
template double beta_search(const ModelArguments<double> &);
