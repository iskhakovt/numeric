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

static bool py_to_tabulated(PyObject *listX, PyObject *listY, Tabulated *ret) {
    std::vector<double> x, y;
    if (!py_to_vector(listX, &x) || !py_to_vector(listY, &y)) {
        return false;
    }

    if (x.size() != y.size()) {
        PyErr_SetString(PyExc_ValueError, "lists have different sizes");
    }

    *ret = Tabulated({x, y});
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
    PyObject *x, *y = nullptr;
    if (!PyArg_ParseTuple(args, "O!O!", &PyList_Type, &x, &PyList_Type, &y)) {
        return nullptr;
    }

    Tabulated ret;
    if (!py_to_tabulated(x, y, &ret)) {
        return nullptr;
    }

    return tabulated_to_py(tabulate_integral(ret));
}



static PyMethodDef NumericMethods[] = {
        {"tabulate",  numeric_tabulate, METH_VARARGS, "tabulate"},
        {"tabulate_integral",  numeric_tabulate_integral, METH_VARARGS, "tabulate integral"},
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
