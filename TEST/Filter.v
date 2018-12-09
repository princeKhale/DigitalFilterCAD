module filter(clk, rst, in, out);
	
/*==== Filter Clock and Reset Instantiations ====*/
	input clk, rst;
	
/*==== Input Section Wire Instantiations ====*/
	wire [31:0]aSectionDelay0_out_to_Multi1_in
	wire [31:0]aSectionMulti1_out_to_Adder0_in
	wire [31:0]aSectionAdder0_out_to_Multi0_in
	wire [31:0]aSectionSectionInput_to_Adder0_in
	wire [31:0]aSectionMulti0_out_to_SectionOutput
	
/*====  Output Section Wire Instantiations ====*/
	wire [31:0]bSectionSectionInput_to_Multi0_in
	wire [31:0]bSectionMulti0_out_to_SectionOutput
	
/*====  Input Section Multiplier Instantiations ====*/
	Multiplier aSectionMulti0(.input0(aSectionAdder0_out_to_Multi0_in), .input1(32'b0000000000000000_0000000000000000), .out(aSectionMulti0_out_to_SectionOutput));
	Multiplier aSectionMulti1(.input0(aSectionDelay0_out_to_Multi1_in), .input1(32'b0000000000000000_1100000000000000), .out(aSectionMulti1_out_to_Adder0_in));
	
/*====  Output Section Multiplier Instantiations ====*/
	/*The first multiplier has to have the input and outputs swapped*/
	Multiplier bSectionMulti0(.input0(bSectionMulti0_out_to_SectionOutput), .input1(32'b0000000000000001_0000000000000000), .out(bSectionSectionInput_to_Multi0_in));
	
/*====  Input Section Adder Instantiations ====*/
	Adders aSectionAdder0(.input0(aSectionSectionInput_to_Adder0_in), .input1(aSectionMulti1_out_to_Adder0_in), .out(aSectionAdder0_out_to_Multi0_in));

	/*==== Input Delay Instantiations ====*/
	Delay aSectionDelay0(.clk(clk), .rst(rst), .in(aSectionMulti0_out_to_SectionOutput), .outMulti(aSectionDelay0_out_to_Multi1_in), .outDelay(supply0)
	/*====  Section Connection Instantiations ====*/
	reg [31:0]SectionConnection;
	assign Multi0_out_to_SectionOutput = SectionConnection;

	always@(posedge clk or negedge rst)
	begin
		SectionConnection = Multi0_out_to_SectionOutput;
	end
endmodule