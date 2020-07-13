#include "LZ78.hpp"
#include<iostream>

int main() {

    std::vector<u8> data;
    for(int i=0; i<20; i++) data.push_back((u8)'a');
    
    std::vector<Code> encoded;

    std::cout << "input data size: " << data.size() * sizeof(u8) << " (byte)" << std::endl;

    if(LZWEncode(data, encoded)) {
        std::cout << "--------------------------------------------------------------\n";
        for(auto &e: encoded) {
            std::cout << e << " ";
            for(int i=31; i>=0; i--) {
                std::cout << ((e >> i) & 1);
            }
            std::cout << "\n";
        }

        std::cout << "--------------------------------------------------------------\n"; 
        std::cout << "encoded size: " << encoded.size()*sizeof(Code) << " (byte)" << std::endl; 
    }

    return 0;
}