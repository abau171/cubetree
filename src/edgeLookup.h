#ifndef EDGE_LOOKUP_H
#define EDGE_LOOKUP_H

#include "cube3x3/cube.h"

void genEdgeLookup();

char lookupEdgeDistance(Cube cube);

void cacheEdgeLookup();

void loadEdgeLookup();

#endif