// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_CORE_HPP
#define NUMERIC_CORE_CORE_HPP


#include "polynomial.hpp"
#include "tabulated.hpp"

#include <functional>
#include <vector>


extern Tabulated tabulate(std::function<double(double)>, const std::vector<double> &);

extern Tabulated tabulate_chebyshev(std::function<double(double)>, size_t);

extern Tabulated tabulate_integral(const Tabulated &);


#endif //NUMERIC_CORE_CORE_HPP
