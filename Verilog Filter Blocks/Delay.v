module Delay(clk, rst, in, outMulti, outDelay);

	input clk, rst;
	input signed [31:0]in;
	output wire signed [31:0]outMulti, outDelay;
	
	reg signed [31:0]delay;
	
	assign outMulti = delay;
	assign outDelay = delay;
	
	always @(posedge clk or negedge rst)
	begin
		if(rst == 1'b0)
			delay <= 32'b0000000000000000_0000000000000000;
		else
			delay <= in;
	end
	
endmodule