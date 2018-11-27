module Filter(clk, rst, in, outTrunc);
	
	
	input signed [31:0]in;
	input clk, rst;
	output reg signed [31:0]outTrunc;
	
	
	reg signed [31:0]in0;
	reg signed [31:0]in1;
	reg signed [31:0]in2;
	
	wire signed [63:0] a0_in0;
	wire signed [63:0] a1_in1;
	wire signed [63:0] a2_in2;
	
	wire signed [63:0] v;
	
	
	
	/* Test:
		H(z) = 1/(1 - 0.75z^-1 + 0.25z^-2)
		a0 = 1 
	
	I[n]-------->[x]---(V[n])---->[x]
	              ^      |         ^      
					  |      |         |  
					  a0     |         b0 
					         |
					         |
					 [x]<----
	              ^
					  |
					  a1
	
	
	
	*/
	
	initial 
	begin
		in0 = 32'b0000000000000000_0000000000000000;
		in1 = 32'b0000000000000000_0000000000000000;
		in2 = 32'b0000000000000000_0000000000000000;
	end
	
	reg signed [31:0] a0 =  32'b0000000000000001_0000000000000000;
	reg signed [31:0] a1 = -32'b0000000000000000_1100000000000000;
	reg signed [31:0] a2 =  32'b0000000000000000_0100000000000000;
	reg signed [31:0] b0 =  32'b0000000000000001_0000000000000000;
	

	assign a0_in0 = a0 * in0;
	assign a1_in1 = a1 * in1;
	assign a2_in2 = a2 * in2;
	
	assign v = a0_in0 + a1_in1 _ a2_in2;
	
	always @(posedge clk or negedge rst)
	begin
			if(rst == 1'b0) 
			begin
				out0 <= 32'b0000000000000000_0000000000000000;
				in0 <= 32'b0000000000000000_0000000000000000;
				in1 <= 32'b0000000000000000_0000000000000000;
				in2 <= 32'b0000000000000000_0000000000000000;
			end 
				
			else 
			begin
				in2 <= ;
				in1 <= ;
				in0 <= in;            
			end
	end
	
endmodule 