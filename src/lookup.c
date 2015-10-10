#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

#include <lookup.h>

void saveLookupCache(char* cacheFile, char* lookup, int bytes) {
	FILE* file = fopen(cacheFile, "wb");
	fwrite(lookup, sizeof(char), bytes, file);
	fclose(file);
}

bool loadLookupCache(char* cacheFile, char* lookup, int bytes) {
	if (access(cacheFile, F_OK) != -1) {
		FILE* file = fopen(cacheFile, "rb");
		fread(lookup, sizeof(char), bytes, file);
		fclose(file);
		return true;
	} else {
		return false;
	}
}
