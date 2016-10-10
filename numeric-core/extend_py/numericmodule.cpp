// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "../core.hpp"

#include <Python.h>
#include <cmath>


static PyObject *evalFunc = nullptr;

static double call_func(double arg) {
    PyObject *args = PyTuple_Pack(1, PyFloat_FromDouble(arg));
    PyObject *result = PyObject_CallObject(evalFunc, args);

    if (!result) {
        PyErr_SetString(PyExc_RuntimeError, "call_func error");
        return nan("");
    }

    return PyFloat_AsDouble(result);
}


static PyObject * vector_to_py(const std::vector<double> &arr) {
    PyObject *list = PyList_New(arr.size());

    for (size_t i = 0; i != arr.size(); ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(arr[i]));
    }

    return list;
}

static PyObject * tabulated_to_py(const Tabulated &func) {
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

static bool py_to_tabulated(PyObject *func, Tabulated *ret) {
    PyObject *listX = nullptr, *listY = nullptr;
    if (!PyArg_ParseTuple(func, "O!O!", &PyList_Type, &listX, &PyList_Type, &listY)) {
        return false;
    }

    std::vector<double> x, y;
    if (!py_to_vector(listX, &x) || !py_to_vector(listY, &y)) {
        return false;
    }

    if (x.size() != y.size()) {
        PyErr_SetString(PyExc_ValueError, "lists have different sizes");
        return false;
    }

    *ret = Tabulated({x, y});
    return true;
}


static bool py_to_model(PyObject *args, ModelArguments *retArgs, double *beta) {
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
            &PyList_Type, &uTuple,
            &PyList_Type, &sTuple,
            &PyList_Type, &zTuple,
            &x_0, &y_0, &t)
        ) {
            return false;
        }
    }

    Tabulated u, s, z;

    if (!py_to_tabulated(uTuple, &u)) {
        return false;
    }
    if (!py_to_tabulated(sTuple, &s)) {
        return false;
    }
    if (!py_to_tabulated(zTuple, &z)) {
        return false;
    }

    *retArgs = ModelArguments(u, s, z, x_0, y_0, t);
    return true;
}


static PyObject * numeric_tabulate(PyObject *, PyObject *args)  {
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "On", &evalFunc, &n)) {
        return nullptr;
    }

    return tabulated_to_py(tabulate_chebyshev(call_func, static_cast<size_t>(n)));
}

static PyObject * numeric_tabulate_integral(PyObject *, PyObject *args) {
    PyObject *funcTuple;
    if (!PyArg_ParseTuple(args, "O!", &PyTuple_Type, &funcTuple)) {
        return nullptr;
    }

    Tabulated func;
    if (!py_to_tabulated(funcTuple, &func)) {
        return nullptr;
    }

    return tabulated_to_py(tabulate_integral(func));
}

static PyObject * numeric_model(PyObject *, PyObject *args) {
    ModelArguments modelArgs;
    double beta;

    if (!py_to_model(args, &modelArgs, &beta)) {
        return nullptr;
    }

    return tabulated_to_py(solve_model(modelArgs, beta));
}



static PyMethodDef NumericMethods[] = {
        {"tabulate",  numeric_tabulate, METH_VARARGS, "tabulate"},
        {"tabulate_integral",  numeric_tabulate_integral, METH_VARARGS, "tabulate integral"},
        {"cauchy",  numeric_model, METH_VARARGS, "cauchy"},
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
