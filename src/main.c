#include <stdio.h>

#include "puzzle.h"

void main() {
	void* state = generateStartState();
	printf("%d\n", getNumPossibleMoves());
	printf("%d\n", (*(getStatePruningFunction()))(state));
	printf("%d\n", (*(getMovePruningFunction()))(state, 0));
	printf("%d\n", isSolved(state));
	makeMove(state, 0);
	printf("%d\n", isSolved(state));
	undoMove(state, 0);
	printf("%d\n", isSolved(state));
}
