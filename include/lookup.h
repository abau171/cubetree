#ifndef LOOKUP_H
#define LOOKUP_H

/* lookup functions */
uint8_t lookupCornerDistance(int cornersystem_encoding);
uint8_t lookupUpperEdgeDistance(int edgesystem_encoding);
uint8_t lookupLowerEdgeDistance(int edgesystem_encoding);

/* encoding functions */
int encodeCornerSystem(const cornersystem_t* cs);
int encodeUpperEdgeSystem(const edgesystem_t* cs);
int encodeLowerEdgeSystem(const edgesystem_t* cs);

/* generator functions */
void genCornerLookup();
void genUpperEdgeLookup();
void genLowerEdgeLookup();

/* cache file management functions */
void saveCornerLookup();
void saveUpperEdgeLookup();
void saveLowerEdgeLookup();
void loadCornerLookup();
void loadUpperEdgeLookup();
void loadLowerEdgeLookup();

#endif

