#include <stdlib.h>
#include <stdbool.h>

#include "../puzzle_module.h"
#include "algorithm.h"
#include "solution.h"

static Solution searchDepthRec(struct PuzzleModule module, void* state, int maxDepth, int curDepth);

static Solution searchAllMoves(struct PuzzleModule module, void* state, int maxDepth, int curDepth) {
	for (int moveId = 0; moveId < module.numPossibleMoves; moveId++) {
		if (module.pruneMove(state, moveId)) {
			continue;
		}
		module.makeMove(state, moveId);
		Solution solution = searchDepthRec(module, state, maxDepth, curDepth + 1);
		if (solution != NULL) {
			// moveId should be added to solution move stack here
			solution = pushSolution(solution, moveId);
			return solution;
		}
		module.undoMove(state, moveId);
	}
	return false;
}

static Solution searchDepthRec(struct PuzzleModule module, void* state, int maxDepth, int curDepth) {
	if (curDepth == maxDepth) {
		if (module.isSolved(state)) {
			return newSolution();
		} else {
			return NULL;
		}
	} else if (!module.pruneState(state)) {
		return searchAllMoves(module, state, maxDepth, curDepth);
	} else {
		return false;
	}
}

static Solution searchDepth(struct PuzzleModule module, void* state, int maxDepth) {
	return searchDepthRec(module, state, maxDepth, 0);
}

static Solution solvePuzzle(struct PuzzleModule module, void* state) {
	int maxDepth = -1;
	Solution solution = NULL;
	while (solution == NULL) {
		maxDepth++;
		solution = searchDepth(module, state, maxDepth);
	}
	return solution;
}

void runSolverAlgorithm(struct PuzzleModule module) {
	void* state = module.getStartState();
	Solution solution = solvePuzzle(module, state);
	Solution curSol = solution;
	while (curSol->moveId != -1) {
		printf("%d\n", curSol->moveId);
		curSol = curSol->next;
	}
}
