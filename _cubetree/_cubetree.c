#include <Python.h>

#include <stdbool.h>

#include <cube.h>
#include <lookup.h>
#include <search.h>

typedef struct {
    PyObject_HEAD
    cube_t cube_state;
} _cubetree_CubeObject;

static PyObject*
Cube_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    _cubetree_CubeObject* self;
    self = (_cubetree_CubeObject*) type->tp_alloc(type, 0);
    if (self != NULL) {
        self->cube_state = solved_cube;
    }
    return (PyObject*) self;
}

static PyObject*
Cube_get_facelet(_cubetree_CubeObject* self, PyObject* args) {
    int face;
    int i;
    if (!PyArg_ParseTuple(args, "ii", &face, &i))
        return NULL;
    int facelet = getFaceletCube(&self->cube_state, face, i);
    return Py_BuildValue("i", facelet);
}

static PyObject*
Cube_is_solved(_cubetree_CubeObject* self) {
    if (isSolvedCube(&self->cube_state)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static PyObject*
Cube_turn(_cubetree_CubeObject* self, PyObject* args) {
    uint8_t face;
    int turn_type;
    if (!PyArg_ParseTuple(args, "bi", &face, &turn_type))
        return NULL;
    turnCubeSelf(&self->cube_state, face, turn_type);
    Py_RETURN_NONE;
}

static PyObject*
Cube_shuffle(_cubetree_CubeObject* self, PyObject* args) {
    int iterations;
    if (!PyArg_ParseTuple(args, "i", &iterations))
        return NULL;
    shuffleCube(&self->cube_state, iterations);
    Py_RETURN_NONE;
}

static PyObject* py_cancel_checker;

PyObject* _cubetree_set_py_cancel_checker(PyObject* self, PyObject* args) {
    if (PyArg_ParseTuple(args, "O:callback", &py_cancel_checker)) {
        Py_XINCREF(py_cancel_checker);
        Py_RETURN_NONE;
    }
    return NULL;
}

static bool cancel_checker_proxy(void) {
    PyObject* args = Py_BuildValue("()");
    PyObject* cancel_result = PyEval_CallObject(py_cancel_checker, args);
    Py_DECREF(args);
    bool result = PyObject_IsTrue(cancel_result);
    Py_DECREF(cancel_result);
    return result;
}

static PyObject*
Cube_search_depth(_cubetree_CubeObject* self, PyObject* args) {
    int depth;
    if (!PyArg_ParseTuple(args, "i", &depth))
        return NULL;

    movenode_t* solution_node = searchDepth(&self->cube_state, depth);
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

static PyMethodDef Cube_methods[] = {
    {"is_solved", (PyCFunction) Cube_is_solved, METH_NOARGS,
     "Returns True if the cube is solved, False otherwise."},
    {"turn", (PyCFunction) Cube_turn, METH_VARARGS,
     "Turns the cube, given a face and turn type."},
    {"shuffle", (PyCFunction) Cube_shuffle, METH_VARARGS,
     "Shuffles the cube by turning a random face a number of times."},
    {"get_facelet", (PyCFunction) Cube_get_facelet, METH_VARARGS,
    "Gets the facelet color from a facelet id on the cube."},
    {"search_depth", (PyCFunction) Cube_search_depth, METH_VARARGS,
    "Searches the cube for a solution at a given depth."},
    {NULL} /* Sentinel */
};

static PyTypeObject _cubetree_CubeType = {
    PyVarObject_HEAD_INIT(NULL, 0)
    "_cubetree.Cube",             /* tp_name */
    sizeof(_cubetree_CubeObject), /* tp_basicsize */
    0,                            /* tp_itemsize */
    0,                            /* tp_dealloc */
    0,                            /* tp_print */
    0,                            /* tp_getattr */
    0,                            /* tp_setattr */
    0,                            /* tp_reserved */
    0,                            /* tp_repr */
    0,                            /* tp_as_number */
    0,                            /* tp_as_sequence */
    0,                            /* tp_as_mapping */
    0,                            /* tp_hash  */
    0,                            /* tp_call */
    0,                            /* tp_str */
    0,                            /* tp_getattro */
    0,                            /* tp_setattro */
    0,                            /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT,           /* tp_flags */
    "CubeTree Cube",              /* tp_doc */
    0,                            /* tp_traverse */
    0,                            /* tp_clear */
    0,                            /* tp_richcompare */
    0,                            /* tp_weaklistoffset */
    0,                            /* tp_iter */
    0,                            /* tp_iternext */
    Cube_methods,                 /* tp_methods */
    0,                            /* tp_members */
    0,                            /* tp_getset */
    0,                            /* tp_base */
    0,                            /* tp_dict */
    0,                            /* tp_descr_get */
    0,                            /* tp_descr_set */
    0,                            /* tp_dictoffset */
    0,                            /* tp_init */
    0,                            /* tp_alloc */
    Cube_new,                     /* tp_new */
};

static PyObject*
_cubetree_gen_corner_lookup(PyObject* self) {
    genCornerLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_gen_upper_edge_lookup(PyObject* self) {
    genUpperEdgeLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_gen_lower_edge_lookup(PyObject* self) {
    genLowerEdgeLookup();
    Py_RETURN_NONE;
}

static PyObject*
_cubetree_load_lookups(PyObject* self) {
    loadCornerLookup();
    loadUpperEdgeLookup();
    loadLowerEdgeLookup();
    Py_RETURN_NONE;
}

static PyMethodDef _cubetree_methods[] = {
    {"gen_corner_lookup", (PyCFunction) _cubetree_gen_corner_lookup, METH_NOARGS, "Generates the corner lookup table."},
    {"gen_upper_edge_lookup", (PyCFunction) _cubetree_gen_upper_edge_lookup, METH_NOARGS, "Generates the upper edge lookup table."},
    {"gen_lower_edge_lookup", (PyCFunction) _cubetree_gen_lower_edge_lookup, METH_NOARGS, "Generates the lower edge lookup table."},
    {"load_lookups", (PyCFunction) _cubetree_load_lookups, METH_NOARGS, "Loads the lookup tables into memory."},
    {"set_cancel_checker", (PyCFunction) _cubetree_set_py_cancel_checker, METH_VARARGS, "Sets the cancel checker callback function."},
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

