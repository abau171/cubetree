#include <Python.h>

static PyObject* _cubetree_test(PyObject* self, PyObject* args) {
    const char* command;
    int sts;
    if (!PyArg_ParseTuple(args, "s", &command)) return NULL;
    sts = system(command);
    return Py_BuildValue("i", sts);
}

static PyMethodDef moduleMethods[] = {
    {"test", _cubetree_test, METH_VARARGS, "Execute a shell command."},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef _cubetree = {
    PyModuleDef_HEAD_INIT,
    "_cubetree",
    "doc",
    -1,
    moduleMethods
};

PyMODINIT_FUNC PyInit__cubetree(void) {
    return PyModule_Create(&_cubetree);
}

