#ifndef EDGE_LOOKUP_H
#define EDGE_LOOKUP_H

int encodeUpperEdgeSystem(const edgesystem_t* cs);

void genUpperEdgeLookup();

uint8_t lookupUpperEdgeDistance(int edgesystem_encoding);

void saveUpperEdgeLookupCache();

void loadUpperEdgeLookupCache();

#endif
