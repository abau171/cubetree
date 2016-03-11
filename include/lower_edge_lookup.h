#ifndef LOWER_EDGE_LOOKUP_H
#define LOWER_EDGE_LOOKUP_H

int encodeLowerEdgeSystem(const edgesystem_t* cs);

void genLowerEdgeLookup();

uint8_t lookupLowerEdgeDistance(int edgesystem_encoding);

void saveLowerEdgeLookupCache();

void loadLowerEdgeLookupCache();

#endif
