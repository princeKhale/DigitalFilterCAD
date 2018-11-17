module ECE487FinalProject (clk, in, rst, out);
	input [31:0] in;
	input clk, rst;
	output [31:0] out;
	
	reg signed [31:0] out, z1, z2;
	reg signed [63:0] y,y1;
	reg signed [31:0] a1 = 32'b0000000000000011_0000000000000000;
	reg signed [31:0] a2 = 32'b0000000000000000_1100000000000000;
	reg signed [31:0] b1 = 32'b0000000000000101_1010000000000000;
	
	always @(posedge clk or negedge rst)
	begin
			if(rst==1'b0) begin
				out <= 0;
				z1 <= 0;
				z2 <= 0;
			end else begin
				z2 <= z1;
				z1 <= in;
				y <= in + a1*z1 + a2*z2;
				y1 <= y + b1*z1;
				out <= y1[47:16];
			end
	end
endmodule
