// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_POLYNOMIAL_HPP
#define NUMERIC_LOGIC_POLYNOMIAL_HPP


#include "function.hpp"
#include "tabulated.hpp"

#include <vector>


template <class Real>
class Polynomial : public Function<Real> {
    std::vector<Real> coefs;

public:
    Polynomial() {}
    explicit Polynomial(const std::vector<Real> &coefs) : coefs(coefs) {}

    Polynomial<Real> derivative(size_t) const;

    Real operator()(Real) const override;
};


template <class Real>
class PolynomialNewton : public Function<Real> {
    std::vector<std::vector<Real>> coefs;
    std::vector<Real> grid;

public:
    PolynomialNewton() {}
    PolynomialNewton(const Tabulated<Real> &);

    void add_point(Real, Real);

    Real operator()(Real) const override;
};


#endif //NUMERIC_LOGIC_POLYNOMIAL_HPP
