#include <Python.h>

#include <stdbool.h>

#include <cube.h>
#include <lookup.h>
#include <search.h>

#include <_cubetree_Cube.h>

static PyObject*
Cube_new(PyTypeObject* type, PyObject* args, PyObject* kwds)
{
    _cubetree_CubeObject* self;
    self = (_cubetree_CubeObject*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

static PyObject*
Cube_init(_cubetree_CubeObject* self, PyObject* args)
{
    PyObject* cube_data = NULL;
    if (!PyArg_ParseTuple(args, "|O!", &PyList_Type, &cube_data))
        return NULL;

    if (cube_data == NULL) {
        self->cube_state = solved_cube;
        Py_RETURN_NONE;
    }

    long raw_cube_data[40];
    for (Py_ssize_t i = 0; i < 40; i++) {
        PyObject* item = PyList_GetItem(cube_data, i);
        if (item == NULL)
            return NULL;
        long raw_data = PyLong_AsLong(item);
        if (raw_data == -1 && PyErr_Occurred())
            return NULL;
        raw_cube_data[i] = raw_data;
    }

    for (int i = 0; i < 8; i++) {
        self->cube_state.cornersystem.corners[i].cid = raw_cube_data[2 * i];
        self->cube_state.cornersystem.corners[i].rotation = raw_cube_data[2 * i + 1];
    }
    for (int i = 0; i < 12; i++) {
        self->cube_state.edgesystem.edges[i].eid = raw_cube_data[16 + 2 * i];
        self->cube_state.edgesystem.edges[i].flip = raw_cube_data[17 + 2 * i];
    }
    Py_RETURN_NONE;
}

static PyObject*
Cube_get_state(_cubetree_CubeObject* self)
{
    long raw_cube_data[40];
    for (int i = 0; i < 8; i++) {
        raw_cube_data[2 * i] = self->cube_state.cornersystem.corners[i].cid;
        raw_cube_data[2 * i + 1] = self->cube_state.cornersystem.corners[i].rotation;
    }
    for (int i = 0; i < 12; i++) {
        raw_cube_data[16 + 2 * i] = self->cube_state.edgesystem.edges[i].eid;
        raw_cube_data[17 + 2 * i] = self->cube_state.edgesystem.edges[i].flip;
    }

    PyObject* cube_data = PyList_New(40);
    if (cube_data == NULL)
        return NULL;
    for (Py_ssize_t i = 0; i < 40; i++) {
        PyObject* item = PyLong_FromLong(raw_cube_data[i]);
        if (item == NULL) {
            Py_DECREF(cube_data);
            return NULL;
        }
        PyList_SET_ITEM(cube_data, i, item);
    }
    return cube_data;
}

static PyObject*
Cube_get_facelet(_cubetree_CubeObject* self, PyObject* args)
{
    int face;
    int i;
    if (!PyArg_ParseTuple(args, "ii", &face, &i))
        return NULL;
    int facelet = getFaceletCube(&self->cube_state, face, i);
    return Py_BuildValue("i", facelet);
}

static PyObject*
Cube_is_solved(_cubetree_CubeObject* self)
{
    if (isSolvedCube(&self->cube_state)) {
        Py_RETURN_TRUE;
    } else {
        Py_RETURN_FALSE;
    }
}

static PyObject*
Cube_turn(_cubetree_CubeObject* self, PyObject* args)
{
    uint8_t face;
    int turn_type;
    if (!PyArg_ParseTuple(args, "bi", &face, &turn_type))
        return NULL;
    turnCubeSelf(&self->cube_state, face, turn_type);
    Py_RETURN_NONE;
}

static PyObject*
Cube_shuffle(_cubetree_CubeObject* self, PyObject* args)
{
    int iterations;
    if (!PyArg_ParseTuple(args, "i", &iterations))
        return NULL;
    shuffleCube(&self->cube_state, iterations);
    Py_RETURN_NONE;
}

static PyObject*
Cube_search_depth(_cubetree_CubeObject* self, PyObject* args)
{
    int depth;
    if (!PyArg_ParseTuple(args, "i", &depth))
        return NULL;
    movenode_t* solution_node;
    bool found_solution = searchDepth(&self->cube_state, depth, &solution_node);
    if (found_solution) {
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
    } else {
        Py_RETURN_NONE;
    }
}


static PyMethodDef Cube_methods[] = {
    {"get_state", (PyCFunction) Cube_get_state, METH_NOARGS,
     "Returns a tuple of numbers representing the cube's state."},
    {"is_solved", (PyCFunction) Cube_is_solved, METH_NOARGS,
     "Returns True if the cube is solved, False otherwise."},
    {"turn", (PyCFunction) Cube_turn, METH_VARARGS,
     "Turns the cube, given a face and turn type."},
    {"shuffle", (PyCFunction) Cube_shuffle, METH_VARARGS,
     "Shuffles the cube by turning a random face a number of times."},
    {"get_facelet", (PyCFunction) Cube_get_facelet, METH_VARARGS,
     "Gets the facelet color from a facelet id on the cube."},
    {"search_depth", (PyCFunction) Cube_search_depth, METH_VARARGS,
     "Searches the cube for a solution at a given turn depth."},
    {NULL} /* Sentinel */
};

PyTypeObject _cubetree_CubeType = {
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
    (initproc) Cube_init,         /* tp_init */
    0,                            /* tp_alloc */
    Cube_new,                     /* tp_new */
};

