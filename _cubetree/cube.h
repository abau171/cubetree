#ifndef CUBE_H
#define CUBE_H

#include <stdbool.h>
#include <stdint.h>

#define positive_modulo(x, mod) (((x % mod) + mod) % mod)

#define U_FACE 0
#define L_FACE 1
#define F_FACE 2
#define R_FACE 3
#define B_FACE 4
#define D_FACE 5

#define CLOCKWISE_TURN 1
#define DOUBLE_TURN 2
#define COUNTER_TURN 3


/* cube data structure */
typedef struct {
    uint8_t cid;
    uint8_t rotation;
} corner_t;

typedef struct {
    corner_t corners[8];
} cornersystem_t;

typedef struct {
    uint8_t eid;
    uint8_t flip;
} edge_t;

typedef struct {
    edge_t edges[12];
} edgesystem_t;

typedef struct {
    cornersystem_t cornersystem;
    edgesystem_t edgesystem;
} cube_t;


/* pre-initialized solved cube references */
const cube_t solved_cube;
const cornersystem_t solved_cornersystem;
const edgesystem_t solved_edgesystem;


/* cube operations */
void turnCube(cube_t* destination,
              const cube_t* source,
              uint8_t face,
              int turn_type);
void turnCubeSelf(cube_t* cube,
                  uint8_t face,
                  int turn_type);
void turnCornerSystem(cornersystem_t* destination,
                      const cornersystem_t* source,
                      uint8_t face,
                      int turn_type);
void turnEdgeSystem(edgesystem_t* destination,
                    const edgesystem_t* source,
                    uint8_t face,
                    int turn_type);


/* cube observation functions */
uint8_t getFaceletCube(const cube_t* cube, uint8_t face, uint8_t i);
bool isSolvedCube(const cube_t* cube);
void shuffleCube(cube_t* cube, int iterations);
uint8_t getFaceletCornerSystem(const cornersystem_t* cs, uint8_t face, int i);
bool isSolvedCornerSystem(const cornersystem_t* cs);
uint8_t getFaceletEdgeSystem(const edgesystem_t* es, uint8_t face, int i);
bool isSolvedEdgeSystem(const edgesystem_t* es);


#endif

