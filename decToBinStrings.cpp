#include <bitset>
#include <iostream>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    std::bitset<32>bset1(std::stol(argv[1]));
    std::cout << bset1.to_string() << std::endl;
    
    return 0;
}
