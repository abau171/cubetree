#include <Python.h>

#include <stdbool.h>

#include <cube.h>
#include <lookup.h>
#include <search.h>
#include <_cubetree_Cube.h>

static PyObject* py_cancel_checker;

static PyObject*
_cubetree_set_py_cancel_checker(PyObject* self, PyObject* args)
{
    if (PyArg_ParseTuple(args, "O:callback", &py_cancel_checker)) {
        Py_XINCREF(py_cancel_checker);
        Py_RETURN_NONE;
    }
    return NULL;
}

static bool
cancel_checker_proxy(void)
{
    PyObject* args = Py_BuildValue("()");
    PyObject* cancel_result = PyEval_CallObject(py_cancel_checker, args);
    Py_DECREF(args);
    bool result = PyObject_IsTrue(cancel_result);
    Py_DECREF(cancel_result);
    return result;
}

static PyObject*
_cubetree_search_depth(PyObject* self, PyObject* args)
{
    PyObject* cube_pyobj;
    _cubetree_CubeObject* cube;
    int depth;
    if (!PyArg_ParseTuple(args, "Oi", &cube_pyobj, &depth))
        return NULL;
    if (!PyObject_IsInstance(cube_pyobj, (PyObject*) &_cubetree_CubeType))
        return NULL;
    cube = (_cubetree_CubeObject*) cube_pyobj;
    movenode_t* solution_node = searchDepth(&cube->cube_state, depth);
    if (solution_node == NULL)
        Py_RETURN_NONE;
    PyObject* solution_list = PyList_New(0);
    while (solution_node != NULL) {
        PyObject* move = PyTuple_New(2);

        PyObject* tmp = Py_BuildValue("i", solution_node->face);
        PyTuple_SetItem(move, 0, tmp);

        tmp = Py_BuildValue("i", solution_node->turn_type);
        PyTuple_SetItem(move, 1, tmp);

        PyList_Append(solution_list, move);
        Py_DECREF(move);

        solution_node = solution_node->next_node;
    }
    return solution_list;
}

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
    {"set_cancel_checker", (PyCFunction) _cubetree_set_py_cancel_checker, METH_VARARGS, "Sets the cancel checker callback function."},
    {"search_depth", (PyCFunction) _cubetree_search_depth, METH_VARARGS,
    "Searches the cube for a solution at a given depth."},
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
    set_cancel_checker(&cancel_checker_proxy);
    return m;
}

