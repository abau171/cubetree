#include <Python.h>

#include <stdbool.h>
#include <time.h>

#include <cube.h>
#include <lookup.h>
#include <search.h>
#include <_cubetree_Cube.h>

static PyObject*
_cubetree_gen_corner_lookup(PyObject* self)
{
    genCornerLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_gen_upper_edge_lookup(PyObject* self)
{
    genUpperEdgeLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_gen_lower_edge_lookup(PyObject* self)
{
    genLowerEdgeLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_load_corner_lookup(PyObject* self)
{
    if (loadCornerLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject*
_cubetree_load_upper_edge_lookup(PyObject* self)
{
    if (loadUpperEdgeLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject*
_cubetree_load_lower_edge_lookup(PyObject* self)
{
    if (loadLowerEdgeLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject*
_cubetree_save_corner_lookup(PyObject* self)
{
    if (saveCornerLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject*
_cubetree_save_upper_edge_lookup(PyObject* self)
{
    if (saveUpperEdgeLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyObject*
_cubetree_save_lower_edge_lookup(PyObject* self)
{
    if (saveLowerEdgeLookup())
        Py_RETURN_TRUE;
    else
        Py_RETURN_FALSE;
}

static PyMethodDef _cubetree_methods[] = {
    {"gen_corner_lookup", (PyCFunction) _cubetree_gen_corner_lookup, METH_NOARGS, "Generates the corner lookup table."},
    {"gen_upper_edge_lookup", (PyCFunction) _cubetree_gen_upper_edge_lookup, METH_NOARGS, "Generates the upper edge lookup table."},
    {"gen_lower_edge_lookup", (PyCFunction) _cubetree_gen_lower_edge_lookup, METH_NOARGS, "Generates the lower edge lookup table."},
    {"load_corner_lookup", (PyCFunction) _cubetree_load_corner_lookup, METH_NOARGS, "Loads the corner lookup table into memory."},
    {"load_upper_edge_lookup", (PyCFunction) _cubetree_load_upper_edge_lookup, METH_NOARGS, "Loads the upper edge lookup table into memory."},
    {"load_lower_edge_lookup", (PyCFunction) _cubetree_load_lower_edge_lookup, METH_NOARGS, "Loads the lower edge lookup table into memory."},
    {"save_corner_lookup", (PyCFunction) _cubetree_save_corner_lookup, METH_NOARGS, "Saves the corner lookup table to a file."},
    {"save_upper_edge_lookup", (PyCFunction) _cubetree_save_upper_edge_lookup, METH_NOARGS, "Saves the upper edge lookup table to a file"},
    {"save_lower_edge_lookup", (PyCFunction) _cubetree_save_lower_edge_lookup, METH_NOARGS, "Saves the lower edge lookup table to a file."},
    {NULL, NULL, 0, NULL}
};

static PyModuleDef _cubetreemodule = {
    PyModuleDef_HEAD_INIT,
    "_cubetree",
    "CubeTree C extensions.",
    -1,
    _cubetree_methods
};

PyMODINIT_FUNC
PyInit__cubetree(void)
{
    //srand(time(NULL));
    srand(0);
    PyObject* m;
    if (PyType_Ready(&_cubetree_CubeType) < 0)
        return NULL;
    m = PyModule_Create(&_cubetreemodule);
    if (m == NULL)
        return NULL;
    Py_INCREF(&_cubetree_CubeType);
    PyModule_AddObject(m, "Cube", (PyObject*) &_cubetree_CubeType);
    return m;
}

