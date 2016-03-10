#ifndef CORNER_H
#define CORNER_H

#include <stdint.h>

typedef struct {
    uint8_t cid;
    uint8_t rotation;
} corner_t;

typedef struct {
    corner_t corners[8];
} cornersystem_t;

const cornersystem_t solved_cornersystem;

void turnCornerSystem(cornersystem_t* destination,
                      const cornersystem_t* source,
                      uint8_t face,
                      int turn_type);

uint8_t getFaceletCornerSystem(const cornersystem_t* cs, uint8_t face, int i);

void printCornerSystem(const cornersystem_t* cs);

#endif
