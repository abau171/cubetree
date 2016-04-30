#include <Python.h>

#include <cube.h>

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
Cube_shuffle(_cubetree_CubeObject* self, PyObject* args) {
    int iterations;
    if (!PyArg_ParseTuple(args, "i", &iterations))
        return NULL;
    shuffleCube(&self->cube_state, iterations);
    Py_RETURN_NONE;
}

static PyMethodDef Cube_methods[] = {
    {"is_solved", (PyCFunction) Cube_is_solved, METH_NOARGS,
     "Returns True if the cube is solved, False otherwise."},
    {"shuffle", (PyCFunction) Cube_shuffle, METH_VARARGS,
     "Shuffles the cube by turning a random face a number of times."},
    {"get_facelet", (PyCFunction) Cube_get_facelet, METH_VARARGS,
    "Gets the facelet color from a facelet id on the cube."},
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

static PyModuleDef _cubetreemodule = {
    PyModuleDef_HEAD_INIT,
    "_cubetree",
    "CubeTree C extensions.",
    -1,
    NULL, NULL, NULL, NULL, NULL
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

