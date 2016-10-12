// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
#define NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP


#include "tabulated.hpp"

#include <stdexcept>

struct ModelArguments {
    Tabulated u;
    Tabulated s;
    Tabulated z;
    double x0;
    double y0;
    double t;

    ModelArguments() {}
    ModelArguments(
        const Tabulated &u,
		const Tabulated &s,
		const Tabulated &z,
		double x0,
		double y0,
		double t
	) : u(u), s(s), z(z), x0(x0), y0(y0), t(t) {
        if (u.get_size() != s.get_size() || s.x != z.x) {
            throw std::invalid_argument("Bad model arguments");
        }
    }
};


#endif //NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
