#include <Python.h>
#include "../ideal.h"

static PyObject * ideal_py(PyObject * self, PyObject * args) {
    double intemp;
    if(!PyArg_ParseTuple(args, "d", &intemp))
        return NULL;
    double out = ideal_pressure(.temp=intemp);
    return Py_BuildValue("d", out);
}

static PyMethodDef method_list[] = {
    {
        "pressure_from_temp", ideal_py, METH_VARARGS,
        "Get the pressure from the temperature of one mole of gunk",
    },
    { NULL, NULL, 0, NULL },
};

static struct PyModuleDef pvnrt_module = {
   PyModuleDef_HEAD_INIT, "pypvnrt", NULL, -1, method_list,
};

PyMODINIT_FUNC
PyInit_pypvnrt(void) {
    return PyModule_Create(&pvnrt_module);
}
