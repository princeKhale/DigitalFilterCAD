module Filter(clk, rst, in, out);
	input [15:0]in;
	input clk, rst;
	output wire [15:0]out;

	reg signed [15:0]testIn = 16'b00000001_00000000;
	reg signed [15:0]out0;
	reg signed [15:0]in0;
	reg signed [15:0]in1;
	reg signed [15:0]in2;
	
	
	/* Test:
		H(z) = 1/(1 - 0.75z^-1 + 0.25z^-2)
		a0 = 1 
	*/
	reg signed [15:0] a1 = -16'b00000000_11000000;
	reg signed [15:0] a2 =  16'b00000000_01000000;
	
	reg signed [15:0] b0 =  16'b00000001_00000000;
	
	assign out = out0;
	
	always @(posedge clk or negedge rst)
	begin
			if(rst == 1'b0) 
			begin
				out0 <= 16'b00000000_00000000;
				in0 <=  16'b00000000_00000000;
				in1 <=  16'b00000000_00000000;
				in2 <=  16'b00000000_00000000;
			end 
				
			else 
			begin
				in2 <= in1;
				in1 <= in0;
				in0 <= testIn;
				
				
				out0 <= b0 * (in0 + (a1 * in1) + (a2 * in2));
			end
	end
	
endmodule 