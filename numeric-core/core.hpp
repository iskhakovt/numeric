// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_CORE_HPP
#define NUMERIC_CORE_CORE_HPP


#include "function.hpp"
#include "matrix.hpp"
#include "model_arguments.hpp"
#include "polynomial.hpp"
#include "tabulated.hpp"

#include <functional>
#include <vector>


const size_t GRID_SIZE = 1000;


template <class Real>
Tabulated<Real> tabulate(const Function<Real> &, const std::vector<Real> &);

template <class Real>
std::vector<Real> chebyshev_roots(size_t, Real, Real);

template <class Real>
Tabulated<Real> tabulate_chebyshev(const Function<Real> &, size_t);

template <class Real>
Polynomial<Real> interpolate(const Tabulated<Real> &);

template <class Real>
Real integral(const Function<Real> &, Real, Real);

template <class Real>
Real integral_gauss_kronrod(const Function<Real> &, Real, Real, size_t);

template <class Real>
Real integral_simpson(const Function<Real> &, Real, Real, size_t);

template <class Real>
Tabulated<Real> tabulate_integral(const Function<Real> &);

template <class Real>
std::vector<Real> linear_system(const Matrix<Real> &, const std::vector<Real> &);

template <class Real>
Tabulated<Real> differential_equation(const ModelArguments<Real> &, Real);

template <class Real>
Tabulated<Real> solve_model(const ModelArguments<Real> &, Real);

template <class Real>
Real beta_search(const ModelArguments<Real> &);


#endif //NUMERIC_CORE_CORE_HPP
