module Multiplier(input0, input1, out);

	input signed [31:0]input0, input1;
	output wire signed [31:0]out;
	//output wire signed [63:0]out64;
	
	wire signed [63:0]multi_res = input0 * input1; 
	assign out = multi_res[47:16];
	//assign out64 = multi_res;

endmodule