// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include <stdexcept>


#include <iostream>

template <class Real>
std::vector<Real> linear_system(const Matrix<Real> &a, const std::vector<Real> &b) {
    std::cerr << "linear_system" << std::endl;

    return {};
}


class LinearSystemException : public std::exception {
	virtual const char* what() const noexcept = 0;
};

class LinearSystemInfiniteSolutions : public LinearSystemException {
	const char* what() const noexcept override { return "infinite solutions"; }
};

class LinearSystemNoSolutions : public LinearSystemException {
	const char* what() const noexcept override { return "no solutions"; }
};


template <class ForwardIt>
ForwardIt max_absolute_element(ForwardIt, ForwardIt);


template <class Real>
std::vector<Real> gaussian_elimination(const Matrix<Real> &_M, const std::vector<Real> &_b) {
	std::cerr << "gaussian_elimination" << std::endl;

	if (_M.size() != _b.size()) {
		throw std::invalid_argument("matrix row size is not equal to vector");
	}

    size_t n = _M.size(), m = _M.row_size();

    if (n < m) {
        throw LinearSystemInfiniteSolutions();
    }

    auto M = _M;
    auto b = _b;

    for (size_t pos = 0; pos != m; ++pos) {
		auto column = M.get_column(pos);

        auto non_zero = max_absolute_element(column.begin() + pos, column.end());
        if (non_zero == column.end() || equal(*non_zero)) {
            throw LinearSystemInfiniteSolutions();
        }

        std::swap(M.matrix[non_zero - column.begin()], M.matrix[pos]);
        std::swap(b[non_zero - column.begin()], b[pos]);

        for (size_t i = pos + 1; i != n; ++i) {
            if (equal(M.matrix[i][pos])) {
                continue;
            }

            Real coef = M.matrix[i][pos] / M.matrix[pos][pos];

            M.matrix[i][pos] = 0.0;
            for (size_t j = pos + 1; j != m; ++j) {
                M.matrix[i][j] -= coef * M.matrix[pos][j];
            }
            b[i] -= coef * b[pos];
        }
    }

    for (size_t pos = m; pos != n; ++pos) {
        if (!equal(b[pos])) {
            throw LinearSystemNoSolutions();
        }
    }

    std::vector<Real> res(m);

    for (ssize_t pos = m - 1; pos >= 0; --pos) {
        res[pos] = b[pos];
        for (size_t i = pos + 1; i != m; ++i) {
            res[pos] -= M.matrix[pos][i] * res[i];
        }
        res[pos] /= M.matrix[pos][pos];
    }

    return res;
}


template <class ForwardIt>
ForwardIt max_absolute_element(ForwardIt begin, ForwardIt end) {
    ForwardIt ret = begin;

    for (auto it = begin; it != end; ++it) {
        if (abs(*it) > abs(*ret)) {
            ret = it;
        }
    }

    return ret;
}


template std::vector<double> linear_system(const Matrix<double> &, const std::vector<double> &);
template std::vector<double> gaussian_elimination(const Matrix<double> &, const std::vector<double> &);
