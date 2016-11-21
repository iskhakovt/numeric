// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


template <class Real>
Tabulated<Real> solve_model(const ModelArguments<Real> &args, Real beta) {
    return {};
}


template <class Real>
Real beta_search(const ModelArguments<Real> &args) {
    return 0.0;
}


template Tabulated<double> solve_model(const ModelArguments<double> &, double);
template double beta_search(const ModelArguments<double> &);
