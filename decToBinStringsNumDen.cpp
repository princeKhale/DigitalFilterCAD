/*Parses multiple input coefficients and provides ability to separate into numerator
 * and denominator coefficients.
 */
#include <bitset>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    /*Determines the number of coefficients belonging to the numerator of the transfer
     * function.  This number MUST be at least 1 for this code to function, as the
     * system response would always be zero if the transfer function is also zero.
     */
    int numCoeffsNumerator = 1;
    for(numCoeffsNumerator; numCoeffsNumerator<argc; numCoeffsNumerator++){
        char *arg = argv[numCoeffsNumerator+1];
        if(arg[0] == '/')
            break;            
    }
    std::cout << numCoeffsNumerator << " coeffs in numerator" << std::endl;
    
    
    
    
    /* Parse each coefficient argument as a double, into a char array of its binary 
     * representation.
     */
    int numCoeffs = argc-2;
    int k = 0, index=0;
    char coefficients[numCoeffs][65];
    for (k; k<argc-1; k++){
        //Get input
        if (k == numCoeffsNumerator){
            continue;
        }
        
        double n = std::stod(argv[k+1]);
        if(abs(n) > 127){
            std::cout << "Error: Coefficient magnitude too large" << std::endl;
            return 1;
        }        
        
        //Split the input into the integer and fractional portions
        bool neg = false;
        if (n<0){
            neg = true;
            n = abs(n);
        }
        int integer = n;
        double fraction = n-integer;
        
        
        //Integer binary digits
        int i = 0;
        while (i<8) {
            coefficients[index][7-i] = '0' + integer % 2; 
            integer = integer / 2;
            i++;
        }
        
        //Binary point
        coefficients[index][8] = '_';
        i++;
        
        //Fractional binary digits
        while (i<65){
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
            
        //Two's complement if the coefficient was negative
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
            for (i=64; i>=0; i--){
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
        for(i=0; i<65; i++){
            std::cout << coefficients[index][i];
        }
        std::cout << std::endl;
        index++;
    }
    
    
    
    
    return 0;
}
