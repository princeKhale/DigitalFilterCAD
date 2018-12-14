#include "main.h"


void writeVerilog(int coeffLength, section_t *input, section_t *output, double optimizeCoeffs[], int isOptimize){
	FILE *mainVerilogFile = fopen("./TEST/OUTPUT/Filter.v", "w");		

	char *rstVal = (char*)malloc(sizeof(char) * coeffLength);
	char zeroRep = '0';
	char rstTerm = '\0';

	int optimize = 0;
	
	for(int i = 0; i < coeffLength; i++){
		//Null terminate	
		if(i == coeffLength - 1)
			rstVal[i] = rstTerm;
		else
			rstVal[i] = zeroRep;
	}
	
	/* Current Coeff this is to keep track through the isZero/isOne arrays*/
	int curCoeff = 0; 
			
	/* Write module declaration */
	fprintf(mainVerilogFile, "%s\n" , "module Filter(clk, rst, in, out);");	

	/* Clk, Rst Instantiations*/
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "==== Filter Port Instantiations ===="); 
	

	fprintf(mainVerilogFile, "	%s%s\n", "input clk,", " rst;");
	
	/* Input instantiation */
	fprintf(mainVerilogFile, "	input signed [%d:0]in;\n", coeffLength - 1);
	
	fprintf(mainVerilogFile, "	output wire signed [%d:0]out;\n", coeffLength - 1);
		
	fprintf(mainVerilogFile, "	assign out = %s%s;\n", output->ID, output->sectionOutput->ID);
			
	fprintf(mainVerilogFile, "	assign %s%s = %s;\n", input->ID, input->sectionInput->ID, "in");
	
	fprintf(mainVerilogFile, "	supply0 [%d:0]%s;\n", coeffLength - 1, "GND");
	
	fprintf(mainVerilogFile, "	wire [%d:0]%s = GND;\n", coeffLength - 1, "endDelayTermination");	
	
	/* Wire Instantiations */
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "==== Input Section Wire Instantiations ===="); 

	/* Input wires */
	for(int i = 0; i < input->numOfEdges; i++){
		fprintf(mainVerilogFile, "	wire signed [%d:0]%s%s;\n", coeffLength - 1, input->ID, input->edges[i]->ID);
	}
	
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Wire Instantiations ===="); 
	
	/* Output wires */ 
	for(int i = 0; i < output->numOfEdges; i++){
		fprintf(mainVerilogFile, "	wire signed [%d:0]%s%s;\n", coeffLength - 1, output->ID, output->edges[i]->ID);
	}


	if(output->numOfMultipliers >= 1)	
		fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Multiplier Instantiations ===="); 
	
	/* Write output Multipliers */ 
	for(int i = 0; i < output->numOfMultipliers; i++){		
		if(isOptimize){
			if(optimizeCoeffs[curCoeff] == 0.0){
				
				if(i == 0)
					fprintf(mainVerilogFile, "	assign %s%s = %d'b%s;\n", output->ID, output->multipliers[i].inputConnect->ID, coeffLength, rstVal);
				else
					fprintf(mainVerilogFile, "	assign %s%s = %d'b%s;\n", output->ID, output->multipliers[i].outputConnect->ID, coeffLength, rstVal);	
		
				curCoeff = curCoeff + 1;

			}else if(optimizeCoeffs[curCoeff] == 1.0){

				if(i == 0)
					fprintf(mainVerilogFile, "	assign %s%s = %s%s;\n", output->ID, output->multipliers[i].inputConnect->ID, output->ID, output->multipliers[i].outputConnect->ID);
				else
					fprintf(mainVerilogFile, "	assign %s%s = %s%s;\n", output->ID, output->multipliers[i].outputConnect->ID, output->ID, output->multipliers[i].inputConnect->ID);
			
				curCoeff = curCoeff + 1;	
			}else{

				/* The first multiplier of the output section needs to have its inputs and outputs flipped */
				if(i == 0){
			
					fprintf(mainVerilogFile, "	/*%s*/\n", "The first multiplier has to have the input and outputs swapped");
					fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
					output->ID,
					output->multipliers[i].ID,
					output->ID, 
					output->multipliers[i].outputConnect->ID,
					coeffLength,
					output->multipliers[i].coefficient,
					output->ID,
					output->multipliers[i].inputConnect->ID
					);
			
				}else{

					fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
					output->ID,
					output->multipliers[i].ID,
					output->ID, 
					output->multipliers[i].inputConnect->ID,
					coeffLength,
					output->multipliers[i].coefficient,
					output->ID,
					output->multipliers[i].outputConnect->ID
					);
	 		       	}
				curCoeff = curCoeff + 1;	
			}


		}else{

			/* The first multiplier of the output section needs to have its inputs and outputs flipped */
			if(i == 0){
			
				fprintf(mainVerilogFile, "	/*%s*/\n", "The first multiplier has to have the input and outputs swapped");
				fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
				output->ID,
				output->multipliers[i].ID,
				output->ID, 
				output->multipliers[i].outputConnect->ID,
				coeffLength,
				output->multipliers[i].coefficient,
				output->ID,
				output->multipliers[i].inputConnect->ID
				);
			}else{

				fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
				output->ID,
				output->multipliers[i].ID,
				output->ID, 
				output->multipliers[i].inputConnect->ID,
				coeffLength,
				output->multipliers[i].coefficient,
				output->ID,
				output->multipliers[i].outputConnect->ID
				);
	 		}
		}
	}
	

	if(input->numOfMultipliers >= 1)
		fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Input Section Multiplier Instantiations ===="); 
	
	/* Write input Multipliers */ 
	for(int i = 0; i < input->numOfMultipliers; i++){
		if(isOptimize){
			if(optimizeCoeffs[curCoeff] == 0.0){
				fprintf(mainVerilogFile, "	assign %s%s = %d'b%s;\n", input->ID, input->multipliers[i].outputConnect->ID, coeffLength, rstVal);		
				curCoeff = curCoeff + 1;
			
			}else if(optimizeCoeffs[curCoeff] == 1.0){
				fprintf(mainVerilogFile, "	assign %s%s = %s%s;\n", input->ID, input->multipliers[i].outputConnect->ID, input->ID, input->multipliers[i].inputConnect->ID);							   curCoeff = curCoeff + 1;
			}else{
				fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
					input->ID,
				input->multipliers[i].ID,
				input->ID, 
				input->multipliers[i].inputConnect->ID,
				coeffLength,
				input->multipliers[i].coefficient,
				input->ID,
				input->multipliers[i].outputConnect->ID
				);
				curCoeff = curCoeff + 1;
			}
		
		}else{


				fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%d'b%s), .out(%s%s));\n", 
				input->ID,
				input->multipliers[i].ID,
				input->ID, 
				input->multipliers[i].inputConnect->ID,
				coeffLength,
				input->multipliers[i].coefficient,
				input->ID,
				input->multipliers[i].outputConnect->ID
				);
	
	
		}
			
	}
	

	if(input->numOfAdders >= 1)
		fprintf(mainVerilogFile, "	\n/*%s*/\n",  "====  Input Section Adder Instantiations ===="); 
		
	/* Write input Adders */
	for(int i = 0; i < input->numOfAdders; i++){
		fprintf(mainVerilogFile, "	Adder %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
		input->ID,
		input->adders[i].ID,
		input->ID, 
		input->adders[i].input0Connect->ID,
		input->ID,
		input->adders[i].input1Connect->ID,
		input->ID,
		input->adders[i].outputConnect->ID
		);
	}

	if(output->numOfAdders >= 1)
		fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Adder Instantiations ===="); 

	/* Write output Adders */
	for(int i = 0; i < output->numOfAdders; i++){
		
		/* Swap the input and outputs of the  first adder of the Output  */
		if(i == 0){
			fprintf(mainVerilogFile, "\n/*	%s */\n\n", "The first adder of the output section must have the inputs and outputs flipped");
			fprintf(mainVerilogFile, "	Adder %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
			output->ID,
			output->adders[i].ID,
			output->ID, 
			output->adders[i].outputConnect->ID,
			output->ID,
			output->adders[i].input1Connect->ID,
			output->ID,
			output->adders[i].input0Connect->ID
			);
	
		}else{
			fprintf(mainVerilogFile, "	Adder %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
			output->ID,
			output->adders[i].ID,
			output->ID, 
			output->adders[i].input0Connect->ID,
			output->ID,
			output->adders[i].input1Connect->ID,
			output->ID,
			output->adders[i].outputConnect->ID
			);
		}
	}
	
	if(input->numOfDelays >= 1)
		fprintf(mainVerilogFile, "\n	/*%s*/\n", "==== Input Delay Instantiations ====");
	
	/* Write input delays */
	for(int i = 0; i < input->numOfDelays; i++){
	
		/* If the last delay the delay output port needs to be grounded */
		if(i == input->numOfDelays - 1){		
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(endDelayTermination));\n",
			input->ID,
			input->delays[i].ID,
			input->ID,
			input->delays[i].inputConnect->ID,
			input->ID,
			input->delays[i].outputMultiConnect->ID
			);		

		}else{
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(%s%s));\n",
			input->ID,
			input->delays[i].ID,
			input->ID,
			input->delays[i].inputConnect->ID,
			input->ID,
			input->delays[i].outputMultiConnect->ID,
			input->ID,
			input->delays[i].outputDelayConnect->ID
			);	
		}
	}
	
	if(output->numOfDelays >= 1)	
		fprintf(mainVerilogFile, "\n	/*%s*/\n", "==== Output Delay Instantiations ====");
	
	/* Write output Delays */
	for(int i = 0; i < output->numOfDelays; i++){
	
		if(i == output->numOfDelays - 1){
		fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(endDelayTermination));\n",
			output->ID,
			output->delays[i].ID,
			output->ID,
			output->delays[i].inputConnect->ID,
			output->ID,
			output->delays[i].outputMultiConnect->ID
			);		
		}else{
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(%s%s));\n",
			output->ID,
			output->delays[i].ID,
			output->ID,
			output->delays[i].inputConnect->ID,
			output->ID,
			output->delays[i].outputMultiConnect->ID,
			output->ID,
			output->delays[i].outputDelayConnect->ID
		);
	       }			
	}
	
			
	/* Connect Sections together and add filter inputs and outputs reversing the input and output of the 2nd (outputSection) */	
	fprintf(mainVerilogFile, "\n	/*%s*/\n",  "====  Section Connection Instantiations ===="); 
	
	fprintf(mainVerilogFile, "	reg signed [%d:0]SectionConnection;\n", coeffLength - 1);

	/* Connect the SectionConnection reg to the input (which was the output in the data structure) of the output section */ 
	fprintf(mainVerilogFile, "	assign %s%s = SectionConnection;\n" , output->ID, output->sectionOutput->ID); 
	
	/* Give SectionConnection Something to latch on */
	fprintf(mainVerilogFile, "\n	always@(posedge clk or negedge rst)\n");
	fprintf(mainVerilogFile, "	%s\n", "begin");

	fprintf(mainVerilogFile, "	%s\n		%s <= %d'b%s;\n", "if(rst == 1'b0)","SectionConnection", coeffLength, rstVal );
	fprintf(mainVerilogFile, "	%s\n		%s <= %s%s;\n", "else",  "SectionConnection", input->ID, input->sectionOutput->ID);
	fprintf(mainVerilogFile, "	%s\n", "end");	

	/* End Module */
	fprintf(mainVerilogFile, "%s", "endmodule");
}	
