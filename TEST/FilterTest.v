module FilterTest(clk, rst, testOut);
	/*Module inputs*/
	input clk, rst;
	output wire [31:0]testOut;
	
	wire posClk = ~clk;
	
	/* Internal Registers */
	reg [7:0]addrCounter;
	
	/* Module Conntections */
	wire [31:0]out;
	wire [15:0]testIn;
	
	Filter TestFiler(.clk(posClk),
						  .rst(rst),
						  .in(testIn), 
						  .out0(out)
						  );
						  
	testROM testROM_inst(.address(addrCounter),
								.clock(posClk),
								.q(testIn)
							  );
						  
	testRAM	testRAM_inst (.address ( addrCounter ),
								  .clock (posClk),
								  .data (out),
								  .wren ( 1'b1 ),
								  .q ( testOut )
								 );

	always @(posedge posClk or negedge rst)
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