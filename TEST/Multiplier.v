module Multiplier(a, b, c);
	input [63:0]a, b;
	output wire c;
	
	assign c = a * b;

endmodule