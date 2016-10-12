// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#ifndef NUMERIC_LOGIC_MATRIX_HPP
#define NUMERIC_LOGIC_MATRIX_HPP


#include <stdexcept>
#include <vector>


struct Matrix {
    std::vector<std::vector<double>> matrix;

    Matrix() {}
    Matrix(std::vector<std::vector<double>> const &matrix) : matrix(matrix) {
        for (auto const & row : matrix) {
            if (row.size() != matrix.front().size()) {
                throw std::invalid_argument("matrix row different sizes");
            }
        }
    }
};


#endif //NUMERIC_LOGIC_MATRIX_HPP
