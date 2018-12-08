

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
