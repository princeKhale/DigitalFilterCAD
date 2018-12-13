# DigitalFilterCAD
A CAD tool that aids in the implementation of digital filters on FPGAs with Verilog.

## Input Format:
./executable integer_digits fractional_digits numerator_coefficient (...) / (denominator_coefficient (...)) optimize_true
### Arguments:
  #### integer_digits
    Type: int
    Meaning: Number of binary digits before the binary point, representing integer digits of the input and output.  REQUIRED for the program to run.
  #### fractional_digits
    Type: int
    Meaning: Number of binary digits after the binary point, representing fractional digits of the input and output.  REQUIRED
  #### numerator_coefficient (...)
    Type: Double
    Meaning: Coefficients for the feedforward portion of a digital FIR/IIR filter of Direct Form II. One coefficient of any value is REQUIRED for the program to run.  Any number of additional coefficients may be included before the "/", separated by spaces.
  #### /
    Meaning: Marks the end of the numerator coefficients and the beginning of any denominator coefficients.  REQUIRED for the program to run.
  #### (denominator_coefficient (...))
    Type: Double
    Meaning: Coefficients for the feedback portion of a digital FIR/IIR filter of Direct Form II. Any number of denominator coefficients may be included after the "/", separated by spaces.  Excluding any denominator coefficients will result in in FIR filter.
  #### optimize_true
    Type: Int
    Meaning: Enable logical optimizations to reduce filter area on the board. "0" will disable optimization; any nonzero value will enable optimization.  REQUIRED for the program to run.
