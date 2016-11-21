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


template <class Real>
std::vector<Real> tridiagonal_thomas(
    const std::vector<Real> &A,
    const std::vector<Real> &C,
    const std::vector<Real> &B,
    const std::vector<Real> &F)
{
    if (A.size() != C.size() || A.size() != B.size() || A.size() != F.size()) {
        throw std::invalid_argument("Invalid tridiagonal argument sizes");
    }

    size_t n = A.size();

    std::vector<Real> alpha(n), beta(n);
    alpha[0] = 0.0;
    beta[0] = 0.0;

    Real z;
    for (size_t i = 1; i != n; ++i) {
        z = (A[i - 1] * alpha[i - 1] + C[i - 1]);
        alpha[i] = -B[i - 1] / z;
        beta[i] = (F[i - 1] - A[i - 1] * beta[i - 1]) / z;
    }

    std::vector<Real> ret(n);
    ret[n - 1] = (F[n - 1] - A[n - 1] * beta[n - 1]) / (C[n - 1] + A[n - 1] * alpha[n - 1]);

    for (ssize_t i = n - 2; i >= 0; --i) {
        ret[i] = alpha[i + 1] * ret[i + 1] + beta[i + 1];
    }

    return ret;
}


template std::vector<double> linear_system(const Matrix<double> &, const std::vector<double> &);
template std::vector<double> gaussian_elimination(const Matrix<double> &, const std::vector<double> &);
template std::vector<double> tridiagonal_thomas(const std::vector<double> &, const std::vector<double> &,
    const std::vector<double> &, const std::vector<double> &);
