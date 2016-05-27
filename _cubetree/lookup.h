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
void genCornerLookup(void);
void genUpperEdgeLookup(void);
void genLowerEdgeLookup(void);

/* cache file management functions */
int loadCornerLookup(void);
int loadUpperEdgeLookup(void);
int loadLowerEdgeLookup(void);

#endif

