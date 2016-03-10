#ifndef CUBE_H
#define CUBE_H

#include <stdint.h>

#include <corner.h>
#include <edge.h>

typedef struct {
    cornersystem_t cornersystem;
    edgesystem_t edgesystem;
} cube_t;

const cube_t solved_cube;

void turnCube(cube_t* destination,
              const cube_t* source,
              uint8_t face,
              int turn_type);

uint8_t getFaceletCube(const cube_t* cube, uint8_t face, uint8_t i);

void printCube(const cube_t* cube);

#endif
