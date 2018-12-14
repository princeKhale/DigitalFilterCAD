#include <bitset>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include <string.h>
#include "structures.h"


/* Function Prototypes */ 

edge_t** addNewEdge(uint8_t curNumberOfEdges, edge_t** oldEdgeList, edge_t* edgeToAdd);

void buildStructuresHelper(int coeffLength, int numOfNumeratorCoeffs, int numOfDenomenatorCoeffs, char** coeffs, double optimizeCoeffs[], int isOptimize, char *filename);
	
section_t* buildSectionStructure(uint8_t numOfCoefficients, char **coefficents);

void writeVerilog(int coeffLength, section_t* input, section_t* output, double optimizeCoeffs[], int isOptimize, char *filename);


