module Adder(a, b, c);
	input [63:0]a, b;
	output wire [63:0]c;
	
	assign c = a + b; 
	
endmodule
