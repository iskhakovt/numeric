// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_CORE_FUNCTION_HPP
#define NUMERIC_CORE_FUNCTION_HPP


template <class Real>
struct Function {
    virtual Real operator()(Real) const = 0;
};


#endif //NUMERIC_CORE_FUNCTION_HPP
