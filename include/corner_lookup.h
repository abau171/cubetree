#ifndef CORNER_LOOKUP_H
#define CORNER_LOOKUP_H

int encodeCornerSystem(const cornersystem_t* cs);

void genCornerLookup();

uint8_t lookupCornerDistance(int cornersystem_encoding);

void saveCornerLookupCache();

void loadCornerLookupCache();

#endif
