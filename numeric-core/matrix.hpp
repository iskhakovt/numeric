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
    Matrix(std::vector<std::vector<Real>> const &matrix) : matrix(matrix) {
        for (auto const & row : matrix) {
            if (row.size() != matrix.front().size()) {
                throw std::invalid_argument("matrix row different sizes");
            }
        }
    }

    const std::vector<double> & get_row(size_t idx) const {
        if (idx >= size()) {
            throw std::invalid_argument("no such row");
        }

        return matrix[idx];
    }

    std::vector<double> get_column(size_t idx) const {
        if (idx >= row_size()) {
            throw std::invalid_argument("no such row");
        }

        std::vector<Real> ret(size());
        for (size_t i = 0; i != size(); ++i) {
            ret[i] = matrix[i][idx];
        }

        return ret;
    }

    size_t size() const {
        return matrix.size();
    }

    size_t row_size() const {
        return matrix.front().size();
    }
};


#endif //NUMERIC_LOGIC_MATRIX_HPP
