// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
#define NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP


#include "tabulated.hpp"

#include <stdexcept>


template <class Real>
struct ModelArguments {
    Tabulated<Real> u;
    Tabulated<Real> s;
    Tabulated<Real> z;
    Real x0;
    Real y0;
    Real t;

    ModelArguments() {}
    ModelArguments(
        const Tabulated<Real> &u,
		const Tabulated<Real> &s,
		const Tabulated<Real> &z,
		Real x0,
		Real y0,
		Real t
	) : u(u), s(s), z(z), x0(x0), y0(y0), t(t) {
        if (u.size() != s.size() || s.x != z.x) {
            throw std::invalid_argument("Bad model arguments");
        }
    }
};


#endif //NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
