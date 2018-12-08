/* Pin to set on the surface of bloks in order for an edge to connect*/
typedef struct pin{
	char *ID;
} pin_t;
	
/* Standard edge to connect blocks */
typedef struct edge{
	char *ID;
	pin_t *feedIn;
	pin_t *feedOut;
} edge_t;

/* Standard Multiplier block */
typedef struct multiplier{
	char *coefficient;
	char *ID;
	edge_t *inputConnect;
	edge_t *outputConnect; 
	pin_t *inputPin;
	pin_t *outputPin;
} multi_t;

/* Standard Delay (Memory Block) */
typedef struct delay{
	char *ID;
	edge_t *inputConnect;
	edge_t *outputMultiConnect; 
	edge_t *outputDelayConnect;
	pin_t *inputPin;
	pin_t *outputPin;
} delay_t;

/* Standard Adder */
typedef struct adder{
	char *ID;
	pin_t *inputPin0;
	pin_t *inputPin1;
	pin_t *outputPin;
	edge_t *input0Connect;
	edge_t *input1Connect;
	edge_t *outputConnect;
} adder_t;


/* Standard Section (a collection of the other block types) */ 
typedef struct section{
	char *ID;
	uint8_t numOfCoefficients;
	uint8_t numOfEdges;
	uint8_t numOfMultipliers;
	uint8_t numOfDelays;
	uint8_t numOfAdders;
	edge_t *sectionOutput;
	edge_t *sectionInput;
	edge_t **edges; 
	multi_t *multipliers;   
	delay_t *delays;
	adder_t *adders;

} section_t;
