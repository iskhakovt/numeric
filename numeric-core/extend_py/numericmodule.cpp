// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include <Python.h>

#include "../core.hpp"

#include <cmath>


class PyFunction : public Function<double> {
    PyObject *evalFunc;

public:
    PyFunction(PyObject *evalFunc) : evalFunc(evalFunc) {}

    double operator()(double arg) const override {
        PyObject *args = PyTuple_Pack(1, PyFloat_FromDouble(arg));
        PyObject *result = PyObject_CallObject(evalFunc, args);

        if (!result) {
            PyErr_SetString(PyExc_RuntimeError, "PyFunction error");
            return nan("");
        }

        return PyFloat_AsDouble(result);
    }
};


static PyObject * vector_to_py(const std::vector<double> &arr) {
    PyObject *list = PyList_New(arr.size());

    for (size_t i = 0; i != arr.size(); ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(arr[i]));
    }

    return list;
}

static PyObject * tabulated_to_py(const Tabulated<double> &func) {
    PyObject *x = vector_to_py(func.x);
    PyObject *y = vector_to_py(func.y);

    return PyTuple_Pack(2, x, y);
}

static bool py_to_vector(PyObject *list, std::vector<double> *ret) {
    if (!PyList_Check(list)) {
        PyErr_SetObject(PyExc_TypeError, list);
        return false;
    }

    ret->resize(static_cast<size_t>(PyList_Size(list)));

    for (size_t i = 0; i != ret->size(); ++i) {
        if (!PyFloat_Check(PyList_GetItem(list, i))) {
        PyErr_SetObject(PyExc_TypeError, PyList_GetItem(list, i));
            return false;
        }
        (*ret)[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    return true;
}

static bool py_to_tabulated(PyObject *func, Tabulated<double> *ret) {
    PyObject *listX = nullptr, *listY = nullptr;
    if (!PyArg_ParseTuple(func, "O!O!", &PyList_Type, &listX, &PyList_Type, &listY)) {
        return false;
    }

    std::vector<double> x, y;
    if (!py_to_vector(listX, &x) || !py_to_vector(listY, &y)) {
        return false;
    }

    try {
        *ret = Tabulated<double>({x, y});
    } catch (std::invalid_argument) {
        PyErr_SetString(PyExc_ValueError, "invalid tabulated format");
        return false;
    }

    return true;
}

static bool py_to_matrix(PyObject *matrix, Matrix<double> *ret) {
    if (!PyList_Check(matrix)) {
        PyErr_SetObject(PyExc_TypeError, matrix);
        return false;
    }

    std::vector<std::vector<double>> ret_matrix(static_cast<size_t>(PyList_Size(matrix)));
    for (size_t i = 0; i != ret_matrix.size(); ++i) {
        if (!py_to_vector(PyList_GetItem(matrix, i), &ret_matrix[i])) {
            return false;
        }
    }

    try {
        *ret = Matrix<double>(ret_matrix);
    } catch (std::invalid_argument) {
        PyErr_SetString(PyExc_ValueError, "invalid matrix format");
        return false;
    }

    return true;
}

static bool py_to_model(PyObject *args, ModelArguments<double> *retArgs, double *beta) {
    PyObject *uTuple = nullptr, *sTuple = nullptr, *zTuple = nullptr;
    double x_0, y_0, t;

    if (beta) {
        if (!PyArg_ParseTuple(
            args,
            "O!O!O!dddd",
            &PyTuple_Type, &uTuple,
            &PyTuple_Type, &sTuple,
            &PyTuple_Type, &zTuple,
            &x_0, &y_0, &t, &beta))
        {
            return false;
        }
    } else {
        if (!PyArg_ParseTuple(
            args,
            "O!O!O!ddd",
            &PyTuple_Type, &uTuple,
            &PyTuple_Type, &sTuple,
            &PyTuple_Type, &zTuple,
            &x_0, &y_0, &t)
        ) {
            return false;
        }
    }

    Tabulated<double> u, s, z;

    if (!py_to_tabulated(uTuple, &u)) {
        return false;
    }
    if (!py_to_tabulated(sTuple, &s)) {
        return false;
    }
    if (!py_to_tabulated(zTuple, &z)) {
        return false;
    }

    *retArgs = ModelArguments<double>(u, s, z, x_0, y_0, t);
    return true;
}


static PyObject * numeric_tabulate_linspace(PyObject *, PyObject *args) {
    try {
        PyObject *evalFunc;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &evalFunc, &a, &b, &n)) {
            return nullptr;
        }

        return tabulated_to_py(tabulate_linspace(PyFunction(evalFunc), a, b, static_cast<size_t>(n)));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_tabulate_chebyshev(PyObject *, PyObject *args) {
    try {
        PyObject *evalFunc;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &evalFunc, &a, &b, &n)) {
            return nullptr;
        }

        return tabulated_to_py(tabulate_chebyshev(PyFunction(evalFunc), a, b, static_cast<size_t>(n)));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_tabulate_polynomial(PyObject *, PyObject *args) {
    try {
        PyObject *funcTuple;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &funcTuple, &a, &b, &n)) {
            return nullptr;
        }

        Tabulated<double> func;
        if (!py_to_tabulated(funcTuple, &func)) {
            return nullptr;
        }

        return tabulated_to_py(tabulate_polynomial(func, a, b, static_cast<size_t>(n)));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_tabulate_spline(PyObject *, PyObject *args) {
    try {
        PyObject *funcTuple;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &funcTuple, &a, &b, &n)) {
            return nullptr;
        }

        Tabulated<double> func;
        if (!py_to_tabulated(funcTuple, &func)) {
            return nullptr;
        }

        return tabulated_to_py(tabulate_spline(func, a, b, static_cast<size_t>(n)));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_integral_gauss_kronrod(PyObject *, PyObject *args) {
    try {
        PyObject *evalFunc;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &evalFunc, &a, &b, &n)) {
            return nullptr;
        }

        return PyFloat_FromDouble(integral_gauss_kronrod(PyFunction(evalFunc), a, b, n));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_integral_simpson(PyObject *, PyObject *args) {
    try {
        PyObject *evalFunc;
        double a, b;
        Py_ssize_t n;
        if (!PyArg_ParseTuple(args, "Oddn", &evalFunc, &a, &b, &n)) {
            return nullptr;
        }

        return PyFloat_FromDouble(integral_simpson(PyFunction(evalFunc), a, b, n));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_tabulate_integral(PyObject *, PyObject *args) {
    try {
        PyObject *funcTuple;
        if (!PyArg_ParseTuple(args, "O!", &PyTuple_Type, &funcTuple)) {
            return nullptr;
        }

        Tabulated<double> func;
        if (!py_to_tabulated(funcTuple, &func)) {
            return nullptr;
        }

        return tabulated_to_py(tabulate_integral(func));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_gaussian_elimination(PyObject *, PyObject *args) {
    try {
        PyObject *py_matrix, *py_vector;
        if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &py_matrix, &PyList_Type, &py_vector)) {
            return nullptr;
        }

        Matrix<double> matrix;
        std::vector<double> vector;
        if (!py_to_matrix(py_matrix, &matrix) || !py_to_vector(py_vector, &vector)) {
            return nullptr;
        }

        return vector_to_py(gaussian_elimination(matrix, vector));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_lu_decomposition(PyObject *, PyObject *args) {
    try {
        PyObject *py_matrix, *py_vector;
        if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &py_matrix, &PyList_Type, &py_vector)) {
            return nullptr;
        }

        Matrix<double> matrix;
        std::vector<double> vector;
        if (!py_to_matrix(py_matrix, &matrix) || !py_to_vector(py_vector, &vector)) {
            return nullptr;
        }

        return vector_to_py(lu_decomposition(matrix, vector));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_tridiagonal_thomas(PyObject *, PyObject *args) {
    try {
        PyObject *py_a, *py_c, *py_b, *py_f;
        if (!PyArg_ParseTuple(args, "O!O!O!O!",
            &PyList_Type, &py_a, &PyList_Type, &py_c, &PyList_Type, &py_b,
            &PyList_Type, &py_f)) {
            return nullptr;
        }

        std::vector<double> a, c, b, f;;
        if (!py_to_vector(py_a, &a) || !py_to_vector(py_c, &c) ||
            !py_to_vector(py_b, &b) || !py_to_vector(py_f, &f)) {
            return nullptr;
        }

        return vector_to_py(tridiagonal_thomas(a, c, b, f));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}


static PyObject * numeric_model(PyObject *, PyObject *args) {
    try {
        ModelArguments<double> modelArgs;
        double beta;

        if (!py_to_model(args, &modelArgs, &beta)) {
            return nullptr;
        }

        return tabulated_to_py(solve_model(modelArgs, beta));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}

static PyObject * numeric_beta_search(PyObject *, PyObject *args) {
    try {
        ModelArguments<double> modelArgs;
        if (!py_to_model(args, &modelArgs, nullptr)) {
            return nullptr;
        }

        return PyFloat_FromDouble(beta_search(modelArgs));
    } catch (std::exception &err) {
        PyErr_SetString(PyExc_RuntimeError, err.what());
        return nullptr;
    }
}


static PyMethodDef NumericMethods[] = {
        {"tabulate_linspace", numeric_tabulate_linspace, METH_VARARGS, "tabulate_linspace"},
        {"tabulate_chebyshev",  numeric_tabulate_chebyshev, METH_VARARGS, "tabulate_chebyshev"},
        {"tabulate_polynomial",  numeric_tabulate_polynomial, METH_VARARGS, "tabulate_polynomial"},
        {"tabulate_spline",  numeric_tabulate_spline, METH_VARARGS, "tabulate_spline"},
        {"integral_gauss_kronrod",  numeric_integral_gauss_kronrod, METH_VARARGS, "integral_gauss_kronrod"},
        {"integral_simpson",  numeric_integral_simpson, METH_VARARGS, "integral_simpson"},
        {"tabulate_integral",  numeric_tabulate_integral, METH_VARARGS, "tabulate integral"},
        {"gaussian_elimination",  numeric_gaussian_elimination, METH_VARARGS, "gaussian_elimination"},
        {"lu_decomposition", numeric_lu_decomposition, METH_VARARGS, "lu_decomposition"},
        {"tridiagonal_thomas",  numeric_tridiagonal_thomas, METH_VARARGS, "tridiagonal_thomas"},
        {"cauchy",  numeric_model, METH_VARARGS, "cauchy"},
        {"beta_search",  numeric_beta_search, METH_VARARGS, "beta search"},
        {nullptr, nullptr, 0, nullptr} /* Sentinel */
};

static struct PyModuleDef numericmodule = {
        PyModuleDef_HEAD_INIT,
        "numeric",
        nullptr,
        -1,
        NumericMethods
};

PyMODINIT_FUNC
PyInit_numeric(void) {
    return PyModule_Create(&numericmodule);
}
