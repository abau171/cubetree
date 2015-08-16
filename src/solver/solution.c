#include <stdlib.h>

#include "solution.h"

Solution newSolution() {
	Solution sol = malloc(sizeof(struct SolutionNode));
	sol->moveId = -1;
	sol->next = NULL;
	return sol;
}

Solution pushSolution(Solution lastSol, int moveId) {
	Solution sol = newSolution();
	sol->moveId = moveId;
	sol->next = lastSol;
	return sol;
}