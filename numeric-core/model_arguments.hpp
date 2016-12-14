// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
#define NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP


#include "tabulated.hpp"


template <class Real>
struct ModelArguments {
    Tabulated<Real> rho;
    Tabulated<Real> S;
    Tabulated<Real> z;
    Real x0;
    Real y0;
    Real T;

    ModelArguments() {}
    ModelArguments(
        const Tabulated<Real> &rho,
		const Tabulated<Real> &S,
		const Tabulated<Real> &z,
		Real x0,
		Real y0,
		Real T
	) : rho(rho), S(S), z(z), x0(x0), y0(y0), T(T) {}
};


#endif //NUMERIC_LOGIC_MODEL_ARGUMENTS_HPP
