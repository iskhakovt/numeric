// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_CORE_HPP
#define NUMERIC_CORE_CORE_HPP


#include "matrix.hpp"
#include "model_arguments.hpp"
#include "polynomial.hpp"
#include "tabulated.hpp"

#include <functional>
#include <vector>


const size_t GRID_SIZE = 1000;


extern Tabulated tabulate(std::function<double(double)>, const std::vector<double> &);

extern Tabulated tabulate_chebyshev(std::function<double(double)>, size_t);

extern Polynomial interpolate(const Tabulated &);

extern Tabulated tabulate_integral(const Tabulated &);

extern std::vector<double> linear_system(const Matrix &, const std::vector<double> &);

extern Tabulated differential_equation(const ModelArguments &, double);

extern Tabulated solve_model(const ModelArguments &, double);

extern double beta_search(const ModelArguments &);


#endif //NUMERIC_CORE_CORE_HPP
