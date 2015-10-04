#ifndef CORNER_LOOKUP_H
#define CORNER_LOOKUP_H

#include "cube3x3/cube.h"

void genCornerLookup();

char lookupCornerDistance(Cube cube);

void cacheCornerLookup();

void loadCornerLookup();

#endif