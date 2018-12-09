module Adder(input0, input1, out);

	input signed [31:0]input0, input1;
	output wire signed [31:0]out;
	
	assign out = input0 + input1; 
	
endmodule