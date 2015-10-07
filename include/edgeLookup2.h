#ifndef EDGE2_LOOKUP_H
#define EDGE2_LOOKUP_H

#include "cube3x3/cube.h"

void genEdge2Lookup();

char lookupEdge2Distance(Cube cube);

void cacheEdge2Lookup();

void loadEdge2Lookup();

#endif