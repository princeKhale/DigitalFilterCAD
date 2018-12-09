module Delay(clk, rst, in, out);

	input clk, rst;
	input [63:0]in;
	output [63:0]out;
	
	
	always @(posedge clk or negedge rst)
	begin
		if(rst == 1'b0)
			out <= 64'b0000000000000000000000000000000000000000000000000000000000000000;
		else
			out <= in;
		end
	end
	
	
endmodule