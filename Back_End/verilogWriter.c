void writeVerilog(section_t *input, section_t *output){
	FILE *mainVerilogFile = fopen("./TEST/OUTPUT/filter.v", "w");		
	
	/* Write module declaration */
	fprintf(mainVerilogFile, "%s\n" , "module filter(clk, rst, in, out);");	

	/* Clk, Rst Instantiations*/
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "==== Filter Clock and Reset Instantiations ===="); 
	

	fprintf(mainVerilogFile, "	%s%s\n", "input clk,", " rst;");	
	

	/* Wire Instantiations */
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "==== Input Section Wire Instantiations ===="); 

	/* Input wires */
	for(int i = 0; i < input->numOfEdges; i++){
		fprintf(mainVerilogFile, "	wire [63:0]%s%s\n", input->ID, input->edges[i]->ID);
	}
	
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Wire Instantiations ===="); 
	
	/* Output wires */ 
	for(int i = 0; i < output->numOfEdges; i++){
		fprintf(mainVerilogFile, "	wire [63:0]%s%s\n", output->ID, output->edges[i]->ID);
	}
	


	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Input Section Multiplier Instantiations ===="); 
	
	/* Write input Multipliers */ 
	for(int i = 0; i < input->numOfMultipliers; i++){
		fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(8'b%s), .out(%s%s));\n", 
		input->ID,
		input->multipliers[i].ID,
		input->ID, 
		input->multipliers[i].inputConnect->ID,
		input->multipliers[i].coefficient,
		input->ID,
		input->multipliers[i].outputConnect->ID
		);
	}
		
	fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Multiplier Instantiations ===="); 
	
	/* Write output Multipliers */ 
	for(int i = 0; i < output->numOfMultipliers; i++){
				
		/* The first multiplier of the output section needs to have its inputs and outputs flipped */
		if(i == 0){
			
			fprintf(mainVerilogFile, "	/*%s*/\n", "The first multiplier has to have the input and outputs swapped");
			fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(8'b%s), .out(%s%s));\n", 
			output->ID,
			output->multipliers[i].ID,
			output->ID, 
			output->multipliers[i].outputConnect->ID,
			output->multipliers[i].coefficient,
			output->ID,
			output->multipliers[i].inputConnect->ID
			);
		}else{

			fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(8'b%s), .out(%s%s));\n", 
			output->ID,
			output->multipliers[i].ID,
			output->ID, 
			output->multipliers[i].inputConnect->ID,
			output->multipliers[i].coefficient,
			output->ID,
			output->multipliers[i].outputConnect->ID
			);
		}
	}
	
		
	fprintf(mainVerilogFile, "	\n/*%s*/\n",  "====  Input Section Adder Instantiations ===="); 
		
	/* Write input Adders */
	for(int i = 0; i < input->numOfAdders; i++){
		fprintf(mainVerilogFile, "	Adders %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
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

	if(output->numOfAdders != 0){		
		fprintf(mainVerilogFile,"	\n/*%s*/\n",  "====  Output Section Adder Instantiations ===="); 
	}

	/* Write output Adders */
	for(int i = 0; i < output->numOfAdders; i++){
		
		/* Swap the input and outputs of the  first adder of the Output  */
		if(i == 0){
			fprintf(mainVerilogFile, "	%s\n", "The first adder of the output section must have the inputs and outputs flipped");
			fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
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
			fprintf(mainVerilogFile, "	Multiplier %s%s(.input0(%s%s), .input1(%s%s), .out(%s%s));\n", 
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

	/* Write input delays */
	for(int i = 0; i < input->numOfDelays; i++){
		fprintf(mainVerilogFile, "\n	/*%s*/\n", "==== Input Delay Instantiations ====");
		
		/* If the last delay the delay output port needs to be grounded */
		if(i == input->numOfDelays - 1){		
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(supply0)",
			input->ID,
			input->delays[i].ID,
			input->ID,
			input->delays[i].inputConnect->ID,
			input->ID,
			input->delays[i].outputMultiConnect->ID
			);		

		}else{
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(%s%s)",
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

	/* Write output Delays */
	for(int i = 0; i < output->numOfDelays; i++){
		fprintf(mainVerilogFile, "\n	/*%s*/\n", "==== Output Delay Instantiations ====");
		
		if(i == output->numOfDelays - 1){
		fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(supply0)",
			output->ID,
			output->delays[i].ID,
			output->ID,
			output->delays[i].inputConnect->ID,
			output->ID,
			output->delays[i].outputMultiConnect->ID
			);		
		}else{
			fprintf(mainVerilogFile, "	Delay %s%s(.clk(clk), .rst(rst), .in(%s%s), .outMulti(%s%s), .outDelay(%s%s)",
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
	
	fprintf(mainVerilogFile, "	%s\n", "reg [63:0]SectionConnection;" );

	/* Connect the SectionConnection reg to the input (which was the output in the data structure) of the output section */ 
	fprintf(mainVerilogFile, "	assign %s = SectionConnection;\n" , output->sectionOutput->ID); 
	
	/* Give SectionConnection Something to latch on */
	fprintf(mainVerilogFile, "\n	always@(posedge clk or negedge rst)\n");
	fprintf(mainVerilogFile, "	begin\n		SectionConnection = %s;\n", input->sectionOutput->ID);
	fprintf(mainVerilogFile, "	%s\n", "end");	

	/* End Module */
	fprintf(mainVerilogFile, "%s", "endmodule");
}	
