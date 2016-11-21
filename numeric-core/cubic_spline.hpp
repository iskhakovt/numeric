// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_CUBIC_SPLINE_HPP
#define NUMERIC_CORE_CUBIC_SPLINE_HPP


#include "tabulated.hpp"

#include <vector>


template <class Real>
class CubicSpline : public Function<Real> {
    struct SplineTuple {
        Real a, b, c, d;
        SplineTuple() : a(0.0), b(0.0), c(0.0), d(0.0) {}
    };

    std::vector<Real> grid;
    std::vector<SplineTuple> splines;

public:
    CubicSpline() {}
    explicit CubicSpline(const Tabulated<Real> &);

    Real operator()(Real) const override;

    static Tabulated<Real> tabulate_spline(const Tabulated<Real> &, double, double, size_t);
};


#endif //NUMERIC_CORE_CUBIC_SPLINE_HPP