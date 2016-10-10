// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include "../core.hpp"

#include <Python.h>


static PyObject *evalFunc = nullptr;

static double call_func(double arg) {
    PyObject *args = PyTuple_Pack(1, PyFloat_FromDouble(arg));
    PyObject *result = PyObject_CallObject(evalFunc, args);
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

static std::vector<double> py_to_vector(PyObject *list) {
    std::vector<double> arr(static_cast<size_t>(PyList_Size(list)));

    for (size_t i = 0; i != arr.size(); ++i) {
        arr[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    return arr;
}

static Tabulated py_to_tabulated(PyObject *tuple) {
    PyObject *x, *y;
    PyArg_ParseTuple(tuple, "O!O!", &PyList_Type, &x, &PyList_Type, &y);

    return Tabulated({py_to_vector(x), py_to_vector(y)});
}


static PyObject * numeric_tabulate(PyObject *, PyObject *args)  {
    Py_ssize_t n;
    if (!PyArg_ParseTuple(args, "On", &evalFunc, &n)) {
        return nullptr;
    }

    return tabulated_to_py(tabulate_chebyshev(call_func, static_cast<size_t>(n)));
}

static PyObject * numeric_tabulate_integral(PyObject *, PyObject *args) {
    return tabulated_to_py(tabulate_integral(py_to_tabulated(args)));
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
