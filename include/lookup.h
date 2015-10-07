#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdbool.h>

/*
 * Initializes all lookup tables.
 */
void initLookups();

/*
 * Attempts to load each lookup table from the cache files.
 * Returns true on a successful load.
 */
bool loadLookupsIfCached();

/*
 * Generates the lookup tables from scratch.
 */
void genLookups();

/*
 * Caches the lookup tables by saving them to their cache files.
 */
void cacheLookups();

#endif