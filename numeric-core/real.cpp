// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include <cmath>


template <class Real>
bool equal(Real x, Real y) {
    return x == y;
}

template<> bool equal(double x, double y) {
    return std::abs(x - y) < 1e-16;
}


template bool equal(double, double);
