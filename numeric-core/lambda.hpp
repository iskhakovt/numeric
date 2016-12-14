// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


// Thanks to Nikos Athanasiou: http://stackoverflow.com/a/24068396


#ifndef NUMERIC_CORE_LAMBDA_HPP
#define NUMERIC_CORE_LAMBDA_HPP


#include <functional>


template<typename T>
struct memfun_type {
    using type = void;
};

template<typename Ret, typename Class, typename... Args>
struct memfun_type<Ret(Class::*)(Args...) const> {
    using type = std::function<Ret(Args...)>;
};

template<typename F>
typename memfun_type<decltype(&F::operator())>::type
FFL(F const &func) { // Function from lambda !
    return func;
}


#endif //NUMERIC_CORE_LAMBDA_HPP
