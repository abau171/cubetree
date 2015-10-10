#ifndef LOOKUP_H
#define LOOKUP_H

#include <stdbool.h>

void saveLookupCache(char* cacheFile, char* lookup, int bytes);

bool loadLookupCache(char* cacheFile, char* lookup, int bytes);

#endif