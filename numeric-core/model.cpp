// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"
#include "lambda.hpp"

#include <algorithm>


const double BETA_SEARCH_C1_COEF = 1.0;
const double BETA_SEARCH_C2_COEF = 10.0;


template <class Real>
Real C1(const ModelArguments<Real> &args, const Tabulated<Real> &x, const Tabulated<Real> &y) {
    auto t = linspace(0.0, args.T, GRID_SIZE);
    auto y_grid = tabulate_spline(y, 0.0, args.T, GRID_SIZE).y;

    auto spline_x = CubicSpline<Real>(x);
    auto dx = FunctionImplemented<Real>(FFL([&](Real t){ return spline_x.derivative(t); }));
    auto dx_grid = tabulate(dx, t).y;

    std::vector<Real> f(args.rho.size());
    for (size_t i = 0; i != args.rho.size(); ++i) {
        f[i] = args.rho.x[i] * args.rho.y[i];
    }

    auto f_integral = tabulate_integral_tab(Tabulated<Real>(args.rho.x, f));
    auto spline_f_integral = CubicSpline<Real>(f_integral);

    std::vector<Real> g(t.size());
    for (size_t i = 0; i != t.size(); ++i) {
        g[i] = dx_grid[i] * spline_f_integral(y_grid[i]);
    }

    auto integral = tabulate_integral_tab(Tabulated<Real>(t, g));

    return 1.0 - integral.y[0] / (args.T - args.x0);
}

template <class Real>
Real C2(const ModelArguments<Real> &args, const Tabulated<Real> &x) {
    auto spline_S = CubicSpline<Real>(args.S);
    auto spline_x = CubicSpline<Real>(x);

    return fixed_abs(spline_x(args.T) - spline_S(args.T)) / spline_S(args.T);
}

template <class Real>
ModelResults<Real> solve_model(const ModelArguments<Real> &args, Real beta) {
    auto spline_rho = CubicSpline<Real>(args.rho);
    auto spline_S = CubicSpline<Real>(args.S);
    auto spline_z = CubicSpline<Real>(args.z);

    CubicSpline<Real> spline_rho_integral;
    spline_rho_integral = CubicSpline<Real>(tabulate_integral_tab(args.rho));

    auto f = [&](Real t, Real x, Real y) {
        if (y < 0.0) y = 0.0;
        return spline_z.derivative(t) * spline_rho_integral(y);
    };
    auto g = [&](Real _, Real t, Real x) { return beta * (spline_S(t) - x); };

    auto func_f = FunctionThreeArgImplemented<Real>(FFL(f));
    auto func_g = FunctionThreeArgImplemented<Real>(FFL(g));

    auto ret = runge_kutta(func_f, func_g, args.x0, args.y0, args.T, GRID_SIZE);
    Real c1 = C1(args, ret.first, ret.second);
    Real c2 = C2(args, ret.first);

    return ModelResults<Real>(
        ret.first, ret.second, c1, c2, BETA_SEARCH_C1_COEF * c1 + BETA_SEARCH_C2_COEF * c2);
}


template <class Real>
Real beta_search(const ModelArguments<Real> &args, Real beta_begin, Real beta_end) {
    auto grid = linspace(beta_begin, beta_end, GRID_SIZE);

    std::vector<ModelResults<Real>> results(grid.size());
    for (size_t i = 0; i != grid.size(); ++i) {
        results[i] = solve_model(args, grid[i]);
    }

    return grid[std::min_element(results.begin(), results.end()) - results.begin()];
}


template ModelResults<double> solve_model(const ModelArguments<double> &, double);
template double beta_search(const ModelArguments<double> &, double, double);
