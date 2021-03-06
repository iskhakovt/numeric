// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MATRIX_HPP
#define NUMERIC_LOGIC_MATRIX_HPP


#include <stdexcept>
#include <vector>


template <class Real>
struct Matrix {
    std::vector<std::vector<Real>> matrix;

    Matrix() {}
    Matrix(size_t n, size_t m) : matrix(n, std::vector<Real>(m, 0.0)) {}
    Matrix(std::vector<std::vector<Real>> const &matrix) : matrix(matrix) {
        for (auto const & row : matrix) {
            if (row.size() != matrix.front().size()) {
                throw std::invalid_argument("Matrix::Matrix: matrix row different sizes");
            }
        }
    }

    std::vector<double> & get_row(size_t idx) {
        if (idx >= size()) {
            throw std::invalid_argument("Matrix::get_row: invalid index");
        }

        return matrix[idx];
    }

    const std::vector<double> & get_row(size_t idx) const {
        if (idx >= size()) {
            throw std::invalid_argument("Matrix::get_row: invalid index");
        }

        return matrix[idx];
    }

    std::vector<double> get_column(size_t idx) const {
        if (idx >= row_size()) {
            throw std::invalid_argument("Matrix::get_column: invalid index");
        }

        std::vector<Real> ret(size());
        for (size_t i = 0; i != size(); ++i) {
            ret[i] = matrix[i][idx];
        }

        return ret;
    }

    std::vector<double> & operator[](size_t idx) {
        return get_row(idx);
    }

    const std::vector<double> & operator[](size_t idx) const {
        return get_row(idx);
    }

    size_t size() const {
        return matrix.size();
    }

    size_t row_size() const {
        return matrix.front().size();
    }

    Matrix<Real> transponse() const {
        std::vector<std::vector<Real>> ret(row_size(), std::vector<Real>(size()));

        for (size_t i = 0; i != size(); ++i) {
            for (size_t j = 0; j != row_size(); ++j) {
                ret[j][i] = matrix[i][j];
            }
        }

        return Matrix<Real>(ret);
    }
};


#endif //NUMERIC_LOGIC_MATRIX_HPP
