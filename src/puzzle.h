#ifndef PUZZLE_H
#define PUZZLE_H

#include <stdbool.h>

void* generateStartState(); // called once

int getNumPossibleMoves(); // called once

typedef bool (*StatePruningFunction)(void* state); // takes state, returns true if pruned
StatePruningFunction getStatePruningFunction(); // called once

typedef bool (*MovePruningFunction)(void* state, int moveId); // takes state and move id, returns true if pruned
MovePruningFunction getMovePruningFunction(); // called once

void makeMove(void* state, int moveId); // called A LOT

void undoMove(void* state, int moveId); // called A LOT

bool isSolved(void* state); // called A LOT

#endif
