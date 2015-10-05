#include <stdbool.h>

#include "lookup.h"
#include "cornerLookup.h"
#include "edgeLookup.h"
#include "edgeLookup2.h"

void initLookups() {
	loadLookupsIfCached();
	//genLookups();
}

bool loadLookupsIfCached() {
	loadCornerLookup();
	loadEdgeLookup();
	loadEdge2Lookup();
	return false;
}

void genLookups() {
	//genCornerLookup();
	//cacheLookups();
	//genEdgeLookup();
	//cacheEdgeLookup();
	genEdge2Lookup();
	cacheEdge2Lookup();
}

void cacheLookups() {
	cacheCornerLookup();
}