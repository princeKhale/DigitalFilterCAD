//Parses multiple input coefficients

#include <bitset>
#include <iostream>
#include <cstdlib>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int numCoeffs = argc-1;
    int k = 0;
    char coefficients[numCoeffs][65];
    for (k; k<numCoeffs; k++){
        //Get input
        double n = std::stod(argv[k+1]);
        if(abs(n) > 127){
            std::cout << "Error: Coefficient magnitude too large";
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
            coefficients[k][7-i] = '0' + integer % 2; 
            integer = integer / 2;
            i++;
        }
        
        //Binary point
        coefficients[k][8] = '_';
        i++;
        
        //Fractional binary digits
        while (i<65){
        fraction *= 2; 
            int fract_bit = fraction; 
                if (fract_bit){ 
                    fraction -= fract_bit; 
                    coefficients[k][i] = '1'; 
                } else {
                    coefficients[k][i] = '0';
                }
                
                i++;
        }
            
        //Two's complement if the coefficient was negative
        if(neg){
            //Flip ones and zeros
            for (i=0; i<65; i++){
                if(coefficients[k][i] == '1'){
                    coefficients[k][i] = '0';
                } else if(coefficients[k][i] == '0'){
                    coefficients[k][i] = '1';
                }
            }
            
            //Add one
            int carry = 1;
            for (i=64; i>=0; i--){
                if(coefficients[k][i] != '_'){
                    char sum = coefficients[k][i] + carry;
                    if (sum != '2'){
                        coefficients[k][i] = sum;
                        carry = 0;
                    } else {
                        coefficients[k][i] = '0';
                        carry = 1;
                    }
                }
            }
            
        }
        
        
        //Display output
        for(i=0; i<65; i++){
            std::cout << coefficients[k][i];
        }
        std::cout << std::endl;
    }
    
    
    
    
    return 0;
}
