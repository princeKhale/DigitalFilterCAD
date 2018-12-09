#include "helper.c"
#include "filterStructureImplementer.h"


/* Function Prototypes */ 

edge_t** addNewEdge(uint8_t curNumberOfEdges, edge_t** oldEdgeList, edge_t* edgeToAdd);

section_t** buildStructuresHelper(int numOfNumeratorCoeffs, int numOfDenomenatorCoeffs, char *coeffs[]);
	
