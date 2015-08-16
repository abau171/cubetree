#ifndef SOLUTION_H
#define SOLUTION_H

typedef struct SolutionNode {
	int moveId;
	struct SolutionNode* next;
}* Solution;

Solution newSolution();

Solution pushSolution(Solution lastSol, int moveId);

#endif