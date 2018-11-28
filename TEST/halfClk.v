module halfClk(clk, rst, halfclk);

	input clk, rst;
	output reg halfclk;
	
	
	always@(posedge clk or negedge rst)
	begin
		if(rst == 1'b0)
			halfclk <= 1'b0;
			
		else
			halfclk <= ~halfclk;
	end
	
	
endmodule 