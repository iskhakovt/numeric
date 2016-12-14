// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_FUNCTION_HPP
#define NUMERIC_CORE_FUNCTION_HPP


#include <functional>


template <class Real>
struct Function {
    virtual Real operator()(Real) const = 0;
};


template <class Real>
class FunctionImplemented : public Function<Real> {
    std::function<Real(Real)> func;

public:
    FunctionImplemented(const std::function<Real(Real)> &func) : func(func) {}

    Real operator()(Real x) const override {
        return func(x);
    }
};


template <class Real>
struct FunctionThreeArg {
    virtual Real operator()(Real, Real, Real) const = 0;
};

template <class Real>
class FunctionThreeArgImplemented : public FunctionThreeArg<Real> {
    std::function<Real(Real, Real, Real)> func;

public:
    FunctionThreeArgImplemented(const std::function<Real(Real, Real, Real)> &func) : func(func) {}

    Real operator()(Real x, Real y, Real z) const override {
        return func(x, y, z);
    }
};


#endif //NUMERIC_CORE_FUNCTION_HPP
