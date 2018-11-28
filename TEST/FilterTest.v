module FilterTest(clk, rst, trigOut, testOut);
	
	/*Module inputs and outputs*/
	input clk, rst;
	output wire [31:0]testOut;
	output wire trigOut;
	
	wire posClk = ~clk; 
	
	/* Internal Registers */
	reg [7:0]addrCounter;
	
	/* Module Conntections */
	wire [31:0]out;
	wire [31:0]testIn;
	wire memClk;
	
	/* Module Definitions */
	halfClk memoryClk(.clk(posClk),
                     .rst(rst),	
 							.halfclk(memClk)	
							);
	
	Filter TestFiler(.clk(posClk),
						  .rst(rst),
						  .in(testIn), 
						  .outTrunc(out)
						  );
						  
	testROM testROM_inst(.address(addrCounter),
								.clock(memClk),
								.q(testIn)
							  );
						  
	testRAM	testRAM_inst (.address ( addrCounter ),
								  .clock (memClk),
								  .data (out),
								  .wren ( 1'b1 ),
								  .q ( testOut )
								 );
								 
	assign trigOut = memClk;							 
	
	/* Address Counter for ROM and RAM */ 							 
	initial addrCounter = 8'b00000000;	
	
	
	always @(posedge memClk or negedge rst)
	begin
		if(rst == 1'b0)
		begin
			addrCounter <= 8'b00000000;
		end
		
		else
		begin
			if(addrCounter == 8'b11111111)
				addrCounter <= 8'b00000000;
			else
				addrCounter <= addrCounter + 1'b1;
		
		end
	
	end
	
	
endmodule 