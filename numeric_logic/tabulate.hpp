// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_TABULATE_H
#define NUMERIC_LOGIC_TABULATE_H


#include <functional>
#include <vector>


extern std::vector<double>
tabulate(std::function<double(double)>, const std::vector<double> &);


#endif //NUMERIC_LOGIC_TABULATE_H
