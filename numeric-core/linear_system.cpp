// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"


#include <iostream>

template <class Real>
std::vector<Real> linear_system(const Matrix<Real> &a, const std::vector<Real> &b) {
    std::cerr << "linear_system" << std::endl;

    return {};
}



template std::vector<double> linear_system(const Matrix<double> &, const std::vector<double> &);
