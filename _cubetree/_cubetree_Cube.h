#ifndef _CUBETREE_CUBE_H
#define _CUBETREE_CUBE_H

PyTypeObject _cubetree_CubeType;

typedef struct {
    PyObject_HEAD
    cube_t cube_state;
} _cubetree_CubeObject;

#endif

