module Filter(clk, rst, in, out);
	
/*==== Filter Port Instantiations ====*/
	input clk, rst;
	input signed [31:0]in;
	output wire signed [31:0]out;
	assign out = bSectionMulti0_out_to_SectionOutput;
	assign SectionInput_to_Adder0_in = in;
	supply0 GND;
	
/*==== Input Section Wire Instantiations ====*/
	wire signed [31:0]aSectionDelay0_out_to_Multi1_in;
	wire signed [31:0]aSectionDelay1_out_to_Multi2_in;
	wire signed [31:0]aSectionDelay0_out_to_Delay1_in;
	wire signed [31:0]aSectionMulti1_out_to_Adder1_in;
	wire signed [31:0]aSectionMulti2_out_to_Adder1_in;
	wire signed [31:0]aSectionAdder1_out_to_Adder0_in;
	wire signed [31:0]aSectionAdder0_out_to_Multi0_in;
	wire signed [31:0]aSectionSectionInput_to_Adder0_in;
	wire signed [31:0]aSectionMulti0_out_to_SectionOutput;
	
/*====  Output Section Wire Instantiations ====*/
	wire signed [31:0]bSectionSectionInput_to_Multi0_in;
	wire signed [31:0]bSectionMulti0_out_to_SectionOutput;
	
/*====  Input Section Multiplier Instantiations ====*/
	Multiplier aSectionMulti0(.input0(aSectionAdder0_out_to_Multi0_in), .input1(32'b0000000000000001_0000000000000000), .out(aSectionMulti0_out_to_SectionOutput));
	Multiplier aSectionMulti1(.input0(aSectionDelay0_out_to_Multi1_in), .input1(32'b0000000000000000_1100000000000000), .out(aSectionMulti1_out_to_Adder1_in));
	Multiplier aSectionMulti2(.input0(aSectionDelay1_out_to_Multi2_in), .input1(32'b0000000000000000_0100000000000000), .out(aSectionMulti2_out_to_Adder1_in));
	
/*====  Output Section Multiplier Instantiations ====*/
	/*The first multiplier has to have the input and outputs swapped*/
	Multiplier bSectionMulti0(.input0(bSectionMulti0_out_to_SectionOutput), .input1(32'b0000000000000001_0000000000000000), .out(bSectionSectionInput_to_Multi0_in));
	
/*====  Input Section Adder Instantiations ====*/
	Adder aSectionAdder0(.input0(aSectionSectionInput_to_Adder0_in), .input1(aSectionAdder1_out_to_Adder0_in), .out(aSectionAdder0_out_to_Multi0_in));
	Adder aSectionAdder1(.input0(aSectionMulti1_out_to_Adder1_in), .input1(aSectionMulti2_out_to_Adder1_in), .out(aSectionAdder1_out_to_Adder0_in));

	/*==== Input Delay Instantiations ====*/
	Delay aSectionDelay0(.clk(clk), .rst(rst), .in(aSectionMulti0_out_to_SectionOutput), .outMulti(aSectionDelay0_out_to_Multi1_in), .outDelay(aSectionDelay0_out_to_Delay1_in));
	/*==== Input Delay Instantiations ====*/
	Delay aSectionDelay1(.clk(clk), .rst(rst), .in(aSectionDelay0_out_to_Delay1_in), .outMulti(aSectionDelay1_out_to_Multi2_in), .outDelay(GND));
	/*====  Section Connection Instantiations ====*/
	reg signed [31:0]SectionConnection;
	assign bSectionMulti0_out_to_SectionOutput = SectionConnection;

	always@(posedge clk or negedge rst)
	begin
	if(rst == 1'b0)
		SectionConnection <= 32'b0000000000000000_0000000000000000;
	else
		SectionConnection <= aSectionMulti0_out_to_SectionOutput;
	end
endmodule