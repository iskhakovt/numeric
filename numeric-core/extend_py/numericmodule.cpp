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

static PyObject * to_list(const std::vector<double> &arr) {
    PyObject *list = PyList_New(arr.size());

    for (size_t i = 0; i != arr.size(); ++i) {
        PyList_SetItem(list, i, PyFloat_FromDouble(arr[i]));
    }

    return list;
}

static PyObject * tabulated_to_py(const Tabulated &func) {
    PyObject *x = to_list(func.x);
    PyObject *y = to_list(func.y);

    return PyTuple_Pack(2, x, y);
}

static std::vector<double> to_vector(PyObject *list) {
    std::vector<double> arr(static_cast<size_t>(PyList_Size(list)));

    for (size_t i = 0; i != arr.size(); ++i) {
        arr[i] = PyFloat_AsDouble(PyList_GetItem(list, i));
    }

    return arr;
}

static PyObject * numeric_tabulate(PyObject *, PyObject *args)  {
    PyObject *list;
    if (!PyArg_ParseTuple(args, "OO!", &evalFunc, &PyList_Type, &list)) {
        return nullptr;
    }

    std::vector<double> arr(static_cast<size_t>(PyList_Size(list)));
    for (size_t i = 0; i != arr.size(); ++i) {
        arr[i] = PyLong_AsLong(PyList_GetItem(list, i));
    }

    return tabulated_to_py(tabulate(call_func, to_vector(list)));
}


static PyMethodDef NumericMethods[] = {
        {"tabulate",  numeric_tabulate, METH_VARARGS, "Execute a shell command."},
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
