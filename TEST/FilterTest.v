module FilterTest(clk, rst, out);
	input clk, rst;
	
	wire [15:0]testIn = 16'b00000001_00000000;
	Filter TestFiler(.clk(),
						  .rst(),
						  .in(), 
						  .out()
						  );
endmodule 