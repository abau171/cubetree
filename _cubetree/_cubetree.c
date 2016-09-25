#include <Python.h>

#include <cube.h>
#include <lookup.h>
#include <search.h>
#include <profile.h>
#include <_cubetree_Cube.h>

/* Module Functions */

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

static PyObject*
_cubetree_run_cube_turns(PyObject* self, PyObject* args)
{
    int num_turns;
    if (!PyArg_ParseTuple(args, "i", &num_turns))
        return NULL;
    run_cube_turns(num_turns);
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_run_corner_encodes(PyObject* self, PyObject* args)
{
    int num_encodes;
    if (!PyArg_ParseTuple(args, "i", &num_encodes))
        return NULL;
    run_cube_corner_encodes(num_encodes);
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_run_upper_edge_encodes(PyObject* self, PyObject* args)
{
    int num_encodes;
    if (!PyArg_ParseTuple(args, "i", &num_encodes))
        return NULL;
    run_cube_upper_edge_encodes(num_encodes);
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_run_lower_edge_encodes(PyObject* self, PyObject* args)
{
    int num_encodes;
    if (!PyArg_ParseTuple(args, "i", &num_encodes))
        return NULL;
    run_cube_lower_edge_encodes(num_encodes);
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_run_all_encodes(PyObject* self, PyObject* args)
{
    int num_encodes;
    if (!PyArg_ParseTuple(args, "i", &num_encodes))
        return NULL;
    run_cube_all_encodes(num_encodes);
    Py_RETURN_NONE;
}

/* _cubetree Module Definition */

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
    {"run_cube_turns", (PyCFunction) _cubetree_run_cube_turns, METH_VARARGS, "Runs a number of random turns on a cube. Useful for profiling."},
    {"run_corner_encodes", (PyCFunction) _cubetree_run_corner_encodes, METH_VARARGS, "Runs a number of random turns on a cube, encoding its corners at each step. Useful for profiling."},
    {"run_upper_edge_encodes", (PyCFunction) _cubetree_run_upper_edge_encodes, METH_VARARGS, "Runs a number of random turns on a cube, encoding its upper edges at each step. Useful for profiling."},
    {"run_lower_edge_encodes", (PyCFunction) _cubetree_run_lower_edge_encodes, METH_VARARGS, "Runs a number of random turns on a cube, encoding its lower edgess at each step. Useful for profiling."},
    {"run_all_encodes", (PyCFunction) _cubetree_run_all_encodes, METH_VARARGS, "Runs a number of random turns on a cube, encoding the entire cube at each step. Useful for profiling."},
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

