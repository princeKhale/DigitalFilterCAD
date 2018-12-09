

edge_t** addNewEdge(uint8_t curNumberOfEdges, edge_t** oldEdgeList, edge_t* edgeToAdd){
	edge_t **oldEdgeListCopy = (edge_t**)malloc(sizeof(edge_t*) * (curNumberOfEdges + 1));
	

	/* If edge is the first */
	if(curNumberOfEdges == 0){
		oldEdgeListCopy[0] = edgeToAdd;
		//free(oldEdgeList);
		return oldEdgeListCopy;	
	}
	else{
		/* Make Deep Copy of old edges */
		for(int i = 0; i < curNumberOfEdges; i++){
			oldEdgeListCopy[i] = oldEdgeList[i];
		}
	
		oldEdgeListCopy[curNumberOfEdges] = edgeToAdd;
		free(oldEdgeList);
		return oldEdgeListCopy;
	}
}


section_t** buildStructuresHelper(int numOfNumeratorCoeffs, int numOfDenomenatorCoeffs, char *coeffs[]){
	char **numeratorCoeffs;
	char **denomenatorCoeffs;

	numeratorCoeffs = (char**)malloc(sizeof(char*) * numOfNumeratorCoeffs);
	denomenatorCoeffs = (char**)malloc(sizeof(char*) * numOfDenomenatorCoeffs);

	uint8_t curCoeff = 0;

	for(int i = 0; i < numOfNumeratorCoeffs; i++){
		numeratorCoeffs[i] = coeffs[curCoeff];
		curCoeff = curCoeff + 1;
	}	
	
	for(int i = 0; i < numOfDenomenatorCoeffs; i++){
		denomenatorCoeffs[i] = coeffs[curCoeff];
		curCoeff = curCoeff + 1;
	}		
	
	/* We're creating 2 sections in our current implementation */ 
	section_t **ret = (section_t**)malloc(sizeof(section_t*) * 2);	
	

	/* Input Section */
	ret[0] = (section_t*)buildSectionStructure((uint8_t)numOfNumeratorCoeffs, numeratorCoeffs);
	
	/* Output Section */
	ret[1] = (section_t*)buildSectionStructure((uint8_t)numOfDenomenatorCoeffs, denomenatorCoeffs);

	return ret;
}	
