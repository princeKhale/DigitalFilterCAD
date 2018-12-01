module clkCounter(clk, rst, outClk);
	input clk, rst;
	output reg outClk;
	
	reg [15:0]counter;
	
	initial counter = 16'b0000000000000000;
	
	
	/* Set clk to be 50mhz / 192k to get a counter of around ~260 */
	
	always@(posedge clk or negedge rst)
	begin
		if(rst == 1'b0)
			counter <= 16'b0000000000000000;
		else 
		begin
			if(counter == 16'b0000000100000100)
			begin
				outClk <= ~outClk;
				counter <= 16'b0000000000000000;
			end
			else
				counter <= counter + 1'b1;
		end
	
	end

endmodule
