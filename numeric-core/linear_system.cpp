// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "core.hpp"

#include <algorithm>
#include <stdexcept>


template <class Real>
std::vector<Real> linear_system(const Matrix<Real> &M, const std::vector<Real> &b) {
    return lu_decomposition(M, b);
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
std::vector<Real> solve_upper_triangular(const Matrix<Real> &M, const std::vector<Real> &b) {
    if (M.size() != b.size()) {
        throw std::invalid_argument("solve_upper_triangular: matrix size is not equal to vector");
    }

    size_t n = M.row_size();
    std::vector<Real> ret(n);

    for (ssize_t pos = n - 1; pos >= 0; --pos) {
        ret[pos] = b[pos];
        for (size_t i = pos + 1; i != n; ++i) {
            ret[pos] -= M[pos][i] * ret[i];
        }
        ret[pos] /= M[pos][pos];
    }

    return ret;
}


template <class Real>
std::vector<Real> solve_lower_triangular(const Matrix<Real> &_M, const std::vector<Real> &_b) {
    if (_M.size() != _b.size()) {
        throw std::invalid_argument("solve_lower_triangular: matrix size is not equal to vector");
    }

    auto M = _M;
    auto b = _b;

    std::reverse(M.matrix.begin(), M.matrix.end());
    for (auto &row : M.matrix) {
        std::reverse(row.begin(), row.end());
    }

    std::reverse(b.begin(), b.end());
    auto ret = solve_upper_triangular(M, b);
    std::reverse(ret.begin(), ret.end());

    return ret;
}


template <class Real>
bool is_linear_dependent(const std::vector<Real> &a, const std::vector<Real> &b) {
    if (a.size() != b.size()) {
        throw std::invalid_argument("is_linear_dependent vectors have different sizes");
    }

    for (size_t i = 1; i != a.size(); ++i) {
        if (!equal(a[i] * b[0], a[0] * b[i])) {
            return false;
        }
    }
    return true;
}


template <class Real>
void build_linear_basis(Matrix<Real> &M, std::vector<Real> &b) {
    if (M.size() != b.size()) {
        throw std::invalid_argument("build_linear_basis: matrix row size is not equal to vector");
    }

    size_t end = M.size();
    for (size_t i = 0; i != end; ++i) {
        for (size_t j = 0; j != i; ++j) {
            if (is_linear_dependent(M.get_row(i), M.get_row(j))) {
                if (!equal(b[i] * M[j][0], b[j] * M[i][0])) {
                    throw LinearSystemNoSolutions();
                }

                std::swap(M[i], M[end - 1]);
                std::swap(b[i], b[end - 1]);
                --end;
                --i;
                break;
            }
        }
    }
}


template <class Real>
std::vector<Real> gaussian_elimination(const Matrix<Real> &_M, const std::vector<Real> &_b) {
	if (_M.size() != _b.size()) {
		throw std::invalid_argument("gaussian_elimination: matrix size is not equal to vector");
	}

    auto M = _M;
    auto b = _b;

    build_linear_basis(M, b);

    size_t n = M.row_size();

    if (M.size() < n) {
        throw LinearSystemInfiniteSolutions();
    }

    for (size_t pos = 0; pos != n; ++pos) {
		auto column = M.get_column(pos);
        auto next = max_absolute_element(column.begin() + pos, column.end());
        if (equal(*next)) {
            throw LinearSystemInfiniteSolutions();
        }


        std::swap(M[next - column.begin()], M[pos]);
        std::swap(b[next - column.begin()], b[pos]);

        for (size_t i = pos + 1; i != n; ++i) {
            if (equal(M[i][pos])) {
                continue;
            }

            Real coef = M[i][pos] / M[pos][pos];

            M[i][pos] = 0.0;
            for (size_t j = pos + 1; j != n; ++j) {
                M[i][j] -= coef * M[pos][j];
            }
            b[i] -= coef * b[pos];
        }
    }

    return solve_upper_triangular(M, b);
}


template <class Real>
std::vector<Real> lu_decomposition(const Matrix<Real> &_M, const std::vector<Real> &_b) {
    if (_M.size() != _b.size()) {
        throw std::invalid_argument("lu_decomposition: matrix size is not equal to vector");
    }

    auto M = _M;
    auto b = _b;

    build_linear_basis(M, b);

    size_t n = M.row_size();

    if (M.size() < n) {
        throw LinearSystemInfiniteSolutions();
    }

    Matrix<Real> L(n, n), U(n, n);

    for (size_t pos = 0; pos != n; ++pos) {
        L[pos][pos] = 1.0;

        auto column = M.get_column(pos);
        auto next = max_absolute_element(column.begin() + pos, column.end());
        std::swap(M[next - column.begin()], M[pos]);
        std::swap(b[next - column.begin()], b[pos]);

        for (size_t i = 0; i != pos + 1; ++i) {
            U[i][pos] = M[i][pos];
            for (size_t j = 0; j != i; ++j) {
                U[i][pos] -= U[j][pos] * L[i][j];
            }

        }
        if (equal(U[pos][pos])) {
            throw LinearSystemInfiniteSolutions();
        }

        for (size_t i = pos; i != n; ++i) {
            L[i][pos] = M[i][pos];
            for (size_t j = 0; j != pos; ++j) {
                L[i][pos] -= U[j][pos] * L[i][j];
            }
            L[i][pos] /= U[pos][pos];
        }
        if (equal(L[pos][pos])) {
            throw LinearSystemInfiniteSolutions();
        }
    }

    auto sub_b = std::vector<Real>(b.begin(), b.begin() + n);
    return solve_upper_triangular(U, solve_lower_triangular(L, sub_b));
}


template <class Real>
std::vector<Real> tridiagonal_thomas(
    const std::vector<Real> &A,
    const std::vector<Real> &C,
    const std::vector<Real> &B,
    const std::vector<Real> &F)
{
    if (A.size() != C.size() || A.size() != B.size() || A.size() != F.size()) {
        throw std::invalid_argument("tridiagonal_thomas: invalid tridiagonal argument sizes");
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
template std::vector<double> lu_decomposition(const Matrix<double> &, const std::vector<double> &);
template std::vector<double> tridiagonal_thomas(const std::vector<double> &, const std::vector<double> &,
    const std::vector<double> &, const std::vector<double> &);
