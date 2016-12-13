// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


template <class Real>
Real fixed_abs(Real x) {
    return x < Real(0) ? -x : x;
}

template <class Real>
bool equal(Real x, Real y) {
    return x == y;
}

template<> bool equal(double x, double y) {
    return fixed_abs(x - y) < 1e-16;
}


template double fixed_abs(double);
template bool equal(double, double);
