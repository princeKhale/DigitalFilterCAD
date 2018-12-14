#include "main.h"

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


void buildStructuresHelper(int coeffLength, int numOfNumeratorCoeffs, int numOfDenominatorCoeffs, char** coeffs, double optimizeCoeff[], int isOptimize){
	char **numeratorCoeffs;
	char **denominatorCoeffs;

	numeratorCoeffs = (char**)malloc(sizeof(char*) * numOfNumeratorCoeffs);
	denominatorCoeffs = (char**)malloc(sizeof(char*) * numOfDenominatorCoeffs);

	uint8_t curCoeff = 0;

	for(int i = 0; i < numOfNumeratorCoeffs; i++){
		printf("%s\n" , coeffs[curCoeff]);
		numeratorCoeffs[i] = coeffs[curCoeff];
		curCoeff = curCoeff + 1;
	}	
	
	for(int i = 0; i < numOfDenominatorCoeffs; i++){
		denominatorCoeffs[i] = coeffs[curCoeff];
		curCoeff = curCoeff + 1;
	}		
	
	// We're creating 2 sections in our current implementation 
	section_t **ret = (section_t**)malloc(sizeof(section_t*) * 2);	
	

	//Output Section 
	ret[0] = (section_t*)buildSectionStructure((uint8_t)numOfNumeratorCoeffs, numeratorCoeffs);
	char *outputID = (char*)"outSection";
	ret[0]->ID = outputID;	

	//Input Section
	ret[1] = (section_t*)buildSectionStructure((uint8_t)numOfDenominatorCoeffs, denominatorCoeffs);
	char *inputID = (char*)"inSection";
	ret[1]->ID = inputID;

	writeVerilog(coeffLength, ret[1], ret[0], optimizeCoeff, isOptimize);
}

	
/*                                                                                          */
/* Function that takes in the numeber of coefficients and the coefficients themselves, and  */
/* builds a section of the filter in an internal represnentation including all multipliers, */
/* adders, delays, and edges between them. This function then returns a pointer             */
/* to this structure.                                                                       */
/*                                                                                          */

section_t* buildSectionStructure(uint8_t numOfCoefficients, char **coefficients){	
	multi_t *multipliers;
	delay_t *delays;
	adder_t *adders;
	edge_t **edges;

		
	uint8_t numOfMultipliers = numOfCoefficients;
	uint8_t numOfDelays = 	numOfCoefficients -1;		
	uint8_t numOfAdders = numOfMultipliers - 1;
	uint16_t usedEdges = 0;	
		
	/* Section we will return */ 
	section_t *sectionRet = (section_t*)malloc(sizeof(section_t));		

	multipliers = (multi_t*)malloc(sizeof(multi_t) * numOfCoefficients);

	/* For coefficient 0 delay  = 0*/
	delays = (delay_t*)malloc(sizeof(delay_t) * (numOfCoefficients - 1));

	/* One adder for each multiplier*/
	adders = (adder_t*)malloc(sizeof(adder_t) * (numOfCoefficients - 1));


	/*
	 * ID all multipliers and create and assign their pins 
    	 */	
	for(int i = 0; i < numOfMultipliers; i++){
		char *multiID = (char*)malloc(sizeof(char) * (24 + 4));
	
		// (12 + 4) = Multi_ddd + 3 for _in + 1 for null terminatior	
		char *inputPinID =  (char*)malloc(sizeof(char) * (24 + 4 + 4));
		
		// (12 + 4) = Multi_ddd + 4 for _out + 1 for null terminatior
		char *outputPinID = (char*)malloc(sizeof(char) * (24 + 4 + 5));

		/* Multiplier_dd\0 = 12 char d section is dependent on the number 
		 * of coefficients and including the null terminatior
		 */
		
		sprintf(multiID, "Multi%d", i);
		multipliers[i].ID = multiID;
	
		//New input pin
		pin_t *newInputPin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(inputPinID, "Multi%d_in", i);
		newInputPin->ID = inputPinID;
		multipliers[i].inputPin = newInputPin;
		
		//New output pin
		pin_t *newOutputPin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(outputPinID, "Multi%d_out", i);
		newOutputPin->ID = outputPinID;
		multipliers[i].outputPin = newOutputPin;

		/* Add in respective Coefficient */
		char *curCoefficient = (char*)malloc(sizeof(char) * 64);
		sprintf(curCoefficient, "%s", coefficients[i]);
		multipliers[i].coefficient = curCoefficient;
					
	}
	
	/*
	 * ID all delays and create and assign pins
	 */
		
	for(int i = 0; i < numOfDelays; i++){
		/* Delay_dddd\0 = 6 char d section is dependent on the number of 
		 * coefficients and including the null terminatior
		 */

		char *delayID = (char*)malloc(sizeof(char) * (24 + 4));
	
		// (12 + 4) = Delay_ddd + 3 for _in + 1 for null terminatior	
		char *inputPinID = (char*)malloc(sizeof(char) * (24 + 4 + 4));
		
		// (12 + 4) = Delay_ddd + 4 for _out + 1 for null terminatior
		char *outputPinID = (char*)malloc(sizeof(char) * (24 + 4 + 5));

		sprintf(delayID, "Delay%d", i);
		delays[i].ID = delayID;

		//New input pin
		pin_t *newInputPin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(inputPinID, "Delay%d_in", i);
		newInputPin->ID = inputPinID;
		delays[i].inputPin = newInputPin;
		
		//New output pin
		pin_t *newOutputPin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(outputPinID, "Delay%d_out", i);
		newOutputPin->ID = outputPinID;
		delays[i].outputPin = newOutputPin;
		
	}

        /*
	 * ID all adders and create and assign pins
	 */
		
	for(int i = 0; i < numOfAdders; i++){
		/* Adder_dddd\0 = 6 char d section is dependent on the number of 
		 * coefficients and including the null terminatior
		 */

		char *adderID = (char*)malloc(sizeof(char) * (24 + 4));
	
		char *inputPin0ID = (char*)malloc(sizeof(char) * (24 + 4 + 4));
		char *inputPin1ID = (char*)malloc(sizeof(char) * (24 + 4 + 4));
		
		// (12 + 4) = Adder_ddd + 4 for _out + 1 for null terminatior
		char *outputPinID = (char*)malloc(sizeof(char) * (24 + 4 + 5));

		sprintf(adderID, "Adder%d", i);
		adders[i].ID = adderID;

		//New input0 pin
		pin_t *newInput0Pin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(inputPin0ID, "Adder%d_in", i);
		newInput0Pin->ID = inputPin0ID;
		adders[i].inputPin0 = newInput0Pin;
			
		//New input1 pin
		pin_t *newInput1Pin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(inputPin1ID, "Adder%d_in", i);
		newInput1Pin->ID = inputPin1ID;
		adders[i].inputPin1 = newInput1Pin;
		
		//New output pin
		pin_t *newOutputPin = (pin_t*)malloc(sizeof(pin_t));
		sprintf(outputPinID, "Adder%d_out", i);
		newOutputPin->ID = outputPinID;
		adders[i].outputPin = newOutputPin;
		
	}

	

	/* Create edge connection between delay output pins and multiplier input pins, 
	 * then set ID to relect those connections. The first multiplier is a special case
	 * and is excluded.
	 */

	for(int i = 0; i < numOfDelays; i++){
		edge_t *delayToMultiConnection = (edge_t*)malloc(sizeof(edge_t)); 
		pin_t *inputPin = multipliers[i+1].inputPin;
		pin_t *outputPin = delays[i].outputPin;	
		char *curID;

		// 2xPrev pin names 2x(12 + 4) + _to_ and null terminatior
		curID = (char*)malloc(sizeof(char) * 64);

		sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);
		
		delayToMultiConnection->ID = curID;
		delayToMultiConnection->feedIn = outputPin;
		delayToMultiConnection->feedOut = inputPin; 
		multipliers[i+1].inputConnect = delayToMultiConnection;
		delays[i].outputMultiConnect = delayToMultiConnection;

		edges = addNewEdge(usedEdges, edges, delayToMultiConnection);		
		usedEdges = usedEdges + 1;
	}

	/* Create edge connection of delay to delay to connect the delay chain, the 
	 * last delay has no other delay to feed into therefore it is assigned NULL 
	 * for this connection. 
	 */
	
	for(int i = 0; i < numOfDelays; i++){
		if(i == numOfDelays - 1){
			delays[i].outputDelayConnect = NULL;
			break;
		}
		else{
			edge_t *delayToDelayConnection = (edge_t*)malloc(sizeof(edge_t)); 
			pin_t *inputPin = delays[i+1].inputPin;
			pin_t *outputPin = delays[i].outputPin;	
			char *curID;
			
			// 2xPrev pin names 2x(12 + 4) + _to_ and null terminatior
			curID = (char*)malloc(sizeof(char) * 64);
			sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);
		
			delayToDelayConnection->ID = curID;
			delayToDelayConnection->feedIn = outputPin;
			delayToDelayConnection->feedOut = inputPin; 
			delays[i+1].inputConnect = delayToDelayConnection;
			delays[i].outputDelayConnect = delayToDelayConnection;

			edges = addNewEdge(usedEdges, edges, delayToDelayConnection);
			usedEdges = usedEdges + 1;
		}	

	}
		 	
	

	
	/* Create edge connection between multiplier output pins and adder input pin 0,
	 * then set ID to reflect the connection. The first multiplier and adder are 
         * a special case and are excluded. The last adder is also a special case as it 
	 * is feed from two multipliers; 
	 */
	
	for(int i = 1; i < numOfMultipliers; i++){	
		
		/* If i is at the index of the last multiplier connect it to the last adders 
		 * input pin 1
		 */
		if(i == numOfMultipliers - 1){
			edge_t *multiToAdderConnection = (edge_t*)malloc(sizeof(edge_t));
			pin_t *outputPin = multipliers[i].outputPin;
			pin_t *inputPin = adders[i - 1].inputPin1;
			char *curID;

			// 2xPrev pin names 2x(12 + 4) + _to_ and null terminatior
			curID = (char*)malloc(sizeof(char) * 64);
			sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);
			
			multiToAdderConnection->ID = curID;
			multiToAdderConnection->feedIn = outputPin;
			multiToAdderConnection->feedOut = inputPin;
			multipliers[i].outputConnect = multiToAdderConnection;
			adders[i - 1].input1Connect =  multiToAdderConnection;

			edges = addNewEdge(usedEdges, edges, multiToAdderConnection);
			usedEdges = usedEdges + 1;
					
		}else{
			edge_t *multiToAdderConnection = (edge_t*)malloc(sizeof(edge_t));
			pin_t *inputPin = adders[i].inputPin0;
			pin_t *outputPin = multipliers[i].outputPin;
			char *curID;
		
			// 2xPrev pin names 2x(12 + 4) + _to_ and null terminatior
			curID = (char*)malloc(sizeof(char) * 64);
			sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);

			multiToAdderConnection->ID = curID;
			multiToAdderConnection->feedIn = outputPin;
			multiToAdderConnection->feedOut = inputPin;
			multipliers[i].outputConnect = multiToAdderConnection;
			adders[i].input0Connect = multiToAdderConnection;

			edges = addNewEdge(usedEdges, edges, multiToAdderConnection);
			usedEdges = usedEdges + 1;
		}
	
	}

	/* Create edge connection between adders starting with the last and moving towards the 
	 * first, each adder will output to the input1 pin of the previous adder in the adders
         * list. The first adder is a special case and feeds into the frist multiplier.  
    	 */
	
	for(int i = numOfAdders - 1; i >= 0; i--){	
		edge_t *adderToAdderConnection = (edge_t*)malloc(sizeof(edge_t));
		char *curID;

		/* If the first adder connect its output to the first multiplier */
		if(i == 0){
			pin_t *inputPin = multipliers[0].inputPin;
			pin_t *outputPin = adders[i].outputPin;
			
			// 2xPrev pin names 2x(12 + 4) + _to_ and null terminatior
			curID = (char*)malloc(sizeof(char) * 64);
			sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);

			adderToAdderConnection->ID = curID;
			adderToAdderConnection->feedIn = outputPin;
			adderToAdderConnection->feedOut = inputPin;
			adders[i].outputConnect = adderToAdderConnection;
			multipliers[i].inputConnect = adderToAdderConnection;

			edges = addNewEdge(usedEdges, edges, adderToAdderConnection);
			usedEdges = usedEdges + 1;
		
		}else{
			pin_t *inputPin = adders[i - 1].inputPin1;
			pin_t *outputPin = adders[i].outputPin;

			curID = (char*)malloc(sizeof(char) * 64);
			sprintf(curID, "%s_to_%s", outputPin->ID, inputPin->ID);

			adderToAdderConnection->ID = curID;
			adderToAdderConnection->feedIn = outputPin;
			adderToAdderConnection->feedOut = inputPin;
			adders[i].outputConnect = adderToAdderConnection;
			adders[i - 1].input1Connect = adderToAdderConnection;

			edges = addNewEdge(usedEdges, edges, adderToAdderConnection);
			usedEdges = usedEdges + 1;
		

		}
	

	}
	
	/* Add Section input and Output to the first adder and the first multiplier respectively */
	edge_t *sectionInput = (edge_t*)malloc(sizeof(edge_t));
	edge_t *sectionOutput = (edge_t*)malloc(sizeof(edge_t));
  	
		
	char *sectionInputID = (char*)malloc(sizeof(char) * (64));
	char *sectionOutputID = (char*)malloc(sizeof(char) * (64)); 	
	
	/* If there is 1 coefficient the input and output feed directly to and from the first multiplier */
	if(numOfCoefficients == 1){
		sprintf(sectionInputID, "SectionInput_to_%s" , multipliers[0].inputPin->ID);
		sprintf(sectionOutputID, "%s_to_SectionOutput", multipliers[0].outputPin->ID);
		sectionInput->feedOut = multipliers[0].inputPin;	
		sectionInput->ID = sectionInputID;
		sectionOutput->feedIn = multipliers[0].outputPin; 
		sectionOutput->ID = sectionOutputID;
		multipliers[0].inputConnect = sectionInput;
		multipliers[0].outputConnect = sectionOutput;
	}else{
		sprintf(sectionInputID, "SectionInput_to_%s" , adders[0].inputPin0->ID);
		sprintf(sectionOutputID, "%s_to_SectionOutput", multipliers[0].outputPin->ID);
		sectionInput->feedOut = adders[0].inputPin0;	
		sectionInput->ID = sectionInputID;
		sectionOutput->feedIn = multipliers[0].outputPin; 
		sectionOutput->ID = sectionOutputID;
		multipliers[0].outputConnect = sectionOutput;
		adders[0].input0Connect = sectionInput;  
	}

		
	edges = addNewEdge(usedEdges, edges,sectionInput);
	usedEdges = usedEdges + 1;
	edges = addNewEdge(usedEdges, edges, sectionOutput);
	usedEdges = usedEdges + 1;	
 	
	/* Set the first delay in the delay block chain to have the section output edge as its input */
	if(numOfDelays >= 1){
		delays[0].inputConnect = sectionOutput;
	} 

	/* Set the return structure's data a shallow copy here is fine as we just want a 
	 * reference to the structure
	 */
	
	sectionRet->numOfCoefficients = numOfCoefficients;
	sectionRet->numOfMultipliers = numOfMultipliers;
	sectionRet->numOfDelays = numOfDelays;
	sectionRet->numOfAdders = numOfAdders;
	sectionRet->numOfEdges = usedEdges;
	sectionRet->multipliers = multipliers;
	sectionRet->delays = delays;
	sectionRet->adders = adders;
	sectionRet->edges = edges;
	sectionRet->sectionOutput = sectionOutput;
	sectionRet->sectionInput = sectionInput; 
	
	return sectionRet;
}



