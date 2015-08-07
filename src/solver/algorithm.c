#include <stdbool.h>

#include "../puzzle_module.h"

static bool searchDepthRec(struct PuzzleModule module, void* state, int maxDepth, int curDepth);

static bool searchAllMoves(struct PuzzleModule module, void* state, int maxDepth, int curDepth) {
	for (int moveId = 0; moveId < module.numPossibleMoves; moveId++) {
		if (module.pruneMove(state, moveId)) {
			continue;
		}
		module.makeMove(state, moveId);
		bool solved = searchDepthRec(module, state, maxDepth, curDepth + 1);
		if (solved) {
			// moveId should be added to solution move stack here
			return true;
		}
		module.undoMove(state, moveId);
	}
	return false;
}

static bool searchDepthRec(struct PuzzleModule module, void* state, int maxDepth, int curDepth) {
	if (curDepth == maxDepth) {
		return module.isSolved(state);
	} else if (!module.pruneState(state)) {
		return searchAllMoves(module, state, maxDepth, curDepth);
	} else {
		return false;
	}
}

static bool searchDepth(struct PuzzleModule module, void* state, int maxDepth) {
	return searchDepthRec(module, state, maxDepth, 0);
}

void runSolverAlgorithm(struct PuzzleModule module) {
	void* state = module.getStartState();
	int maxDepth = -1;
	bool solved = false;
	while (!solved) {
		maxDepth++;
		solved = searchDepth(module, state, maxDepth);
	}
}
