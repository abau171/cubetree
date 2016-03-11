#ifndef EDGE_H
#define EDGE_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    uint8_t eid;
    uint8_t flip;
} edge_t;

typedef struct {
    edge_t edges[12];
} edgesystem_t;

const edgesystem_t solved_edgesystem;

void turnEdgeSystem(edgesystem_t* destination,
                    const edgesystem_t* source,
                    uint8_t face,
                    int turn_type);

uint8_t getFaceletEdgeSystem(const edgesystem_t* es, uint8_t face, int i);

bool isSolvedEdgeSystem(const edgesystem_t* es);

void printEdgeSystem(const edgesystem_t* es);

#endif
