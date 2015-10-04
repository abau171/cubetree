#include <stdbool.h>

#include "lookup.h"
#include "cornerLookup.h"

void initLookups() {
	loadLookupsIfCached();
	//genLookups();
}

bool loadLookupsIfCached() {
	loadCornerLookup();
	return false;
}

void genLookups() {
	genCornerLookup();
	cacheLookups();
}

void cacheLookups() {
	cacheCornerLookup();
}