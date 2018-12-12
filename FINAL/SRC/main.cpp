/* Purpose: Parses multiple input coefficients and provides ability to separate into numerator
 * and denominator coefficients.
 * 
 * Input format: "./executable integer_digits fractional_digits numeratorCoeff1 (...) / (denominatorCoeff1 (...))"
 * The digit arguments, at least one numerator coefficient value, and the "/" are REQUIRED for program to run.
 * Multiple numerator coefficients and any denominator coefficients are optional arguments.
 */


#include "main.h"


int main(int argc, char *argv[]) {
    /*Determines the number of coefficients belonging to the numerator of the transfer
     * function.  This number MUST be at least 1 for this code to function, as the
     * system response would always be zero if the transfer function is also zero.
     */
    int numCoeffsNumerator = 1;
    for(numCoeffsNumerator; numCoeffsNumerator<argc-2; numCoeffsNumerator++){
        char *arg = argv[numCoeffsNumerator+3];
        if(arg[0] == '/')
            break;
    }
    std::cout << numCoeffsNumerator << " coeffs in numerator" << std::endl;
    
    
    /* The first two arguments allow user to specify number of digits for the inputs
     * and outputs.  This section checks if these digits are invalid and informs the user.
     */
    int int_digits = atoi(argv[1]);
    int fract_digits = atoi(argv[2]);
    if(int_digits<0 || fract_digits<0){
        std::cout << "Warning: Negative width specified. Taking absolute value..." << std::endl;
        int_digits = abs(int_digits);
        fract_digits = abs(fract_digits);
    }
    if(int_digits == 0 && fract_digits == 0) {
        std::cout << "Error: Input and output width specified as zero." << std::endl;
        return 1;
    }
    
    int coeffLength = int_digits + fract_digits + 1;
    
    /* Parse each coefficient argument as a double into a char array of its binary 
     * representation.
     */
    int numCoeffs = argc-4;
    int k = 0, index=0;
    char coefficients[numCoeffs][coeffLength];

    //char **coefficients = (char**)malloc(sizeof(char*) * numCoeffs);
    
    for (k; k<argc-3; k++){
        //Skip over the width arguments and the "/" symbol
        if (k == numCoeffsNumerator){
            continue;
        }
        
        //Ensure that the coefficient listed can be contained in the width specified
        double n = atof(argv[k+3]);
        if(abs(n) > pow(2, int_digits-1)-1){
            std::cout << "Error: Coefficient magnitude too large." << std::endl;
            return 1;
        }        
        
        //Split the input into its integer and fractional portions
        bool neg = false;
        if (n<0){
            neg = true;
            n = abs(n);
        }
        int integer = n;
        double fraction = n-integer;
        
        
        //Convert integer decimal digits into char binary digits
        int i = 0;
        while (i<int_digits) {
            coefficients[index][int_digits-1-i] = '0' + integer % 2; 
            integer = integer / 2;
            i++;
        }
        
        //Insert binary point into the array representation
        coefficients[index][int_digits] = '_';
        i++;
        
        //Convert fractional decimal digits into char binary digits
        while (i<coeffLength){
        fraction *= 2; 
            int fract_bit = fraction; 
                if (fract_bit){ 
                    fraction -= fract_bit; 
                    coefficients[index][i] = '1'; 
                } else {
                    coefficients[index][i] = '0';
                }
                
                i++;
        }
            
        //Take two's complement if the coefficient was negative
        if(neg){
            //Flip ones and zeros
            for (i=0; i<65; i++){
                if(coefficients[index][i] == '1'){
                    coefficients[index][i] = '0';
                } else if(coefficients[index][i] == '0'){
                    coefficients[index][i] = '1';
                }
            }
            
            //Add one
            int carry = 1;
            for (i=coeffLength-1; i>=0; i--){
                if(coefficients[index][i] != '_'){
                    char sum = coefficients[index][i] + carry;
                    if (sum != '2'){
                        coefficients[index][i] = sum;
                        carry = 0;
                    } else {
                        coefficients[index][i] = '0';
                        carry = 1;
                    }
                }
            }
            
        }
        
        
        //Display output
        for(i=0; i<coeffLength; i++){
            std::cout << coefficients[index][i];
        }
        std::cout << std::endl;
        index++;
    }
    int numCoeffsDenominator = index - numCoeffsNumerator;
    std::cout << numCoeffsDenominator << " coeffs in denominator" << std::endl;
    

    /* Make a deep copy reference to the coefficients for passing to the helper function */ 
    char **coeffs = (char**)malloc(sizeof(char*) * numCoeffs);		
    for(int i = 0; i < numCoeffs; i++){
	printf("\n%s\n", "Coeff Transfer");
	char *stringToAdd = (char*)malloc(sizeof(char) * coeffLength);
	for(int j = 0; j < coeffLength; j++){
		char *curCharToAdd = (char*)malloc(sizeof(char));
		*curCharToAdd = coefficients[i][j];
		strcat(stringToAdd, curCharToAdd);
		free(curCharToAdd);
	}

	coeffs[i] = stringToAdd;	
    }
	
 
    /* Call to the helper function which handles back end execution */	
       
    buildStructuresHelper(numCoeffsNumerator, numCoeffsDenominator, coeffs);   
    
    
    
    return 0;
}
