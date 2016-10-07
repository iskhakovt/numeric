// Copyright (c) Timur Iskhakov.
// Distributed under the terms of the MIT License.


#include <Python.h>


static PyMethodDef NumericMethods[] = {
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
