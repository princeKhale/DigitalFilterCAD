module Filter(clk, rst, in, out0);
	
	
	input signed [15:0]in;
	input clk, rst;
	output reg signed [31:0]out0;
	
	
	reg signed [15:0]in0;
	reg signed [15:0]in1;
	reg signed [15:0]in2;
	
	
	
	/* Test:
		H(z) = 1/(1 - 0.75z^-1 + 0.25z^-2)
		a0 = 1 
	*/
	initial 
	begin
		in0 = 16'b00000000_00000000;
		in1 = 16'b00000000_00000000;
		in2 = 16'b00000000_00000000;
	end
	
	reg signed [15:0] a0 =  16'b00000001_00000000;
	reg signed [15:0] a1 = -16'b00000000_11000000;
	reg signed [15:0] a2 =  16'b00000000_01000000;
	reg signed [15:0] b0 =  16'b00000001_00000000;
	
	//Assume b0 == 1
	wire signed [31:0] a0_in0 = a0 * in0;
	wire signed [31:0] a1_in1 = a1 * in1;
	wire signed [31:0] a2_in2 = a2 * in2;
	
	
	always @(posedge clk or negedge rst)
	begin
			if(rst == 1'b0) 
			begin
				out0 <= 32'b0000000000000000_0000000000000000;
				in0 <=  16'b00000000_00000000;
				in1 <=  16'b00000000_00000000;
				in2 <=  16'b00000000_00000000;
			end 
				
			else 
			begin
				in2 <= in1;
				in1 <= in0;
				in0 <= in;
				
				//Assume b0 == 1
				out0 <= (a0_in0 + a1_in1 + a2_in2);
			end
	end
	
endmodule 