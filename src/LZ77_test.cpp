#include "LZ77.hpp"
#include<iostream>
#include<vector>

int main() {
    std::vector<char> data; // = {'a', 'a', 'a', 'a'}; //, 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b', 'r', 'a'};
    for(int i=0; i<20; i++) data.push_back('a');
    
    std::vector<u32> encoded;

    u32 indexBit = 16;
    SlidingWindowSearch<typename std::vector<char>::const_iterator, u32> swSearch;

    std::cout << "input data size: " << data.size() * sizeof(char) << " (byte)" << std::endl;

    
    if(LZ77Encode<char, u32, SlidingWindowSearch>(data, encoded, indexBit, swSearch)) {
        std::cout << "--------------------------------------------------------------\n";
        for(auto &e: encoded) {
            std::cout << e << " ";
            for(int i=31; i>=0; i--) {
                std::cout << ((e >> i) & 1);
            }
            std::cout << "\n";
        }

        std::cout << "--------------------------------------------------------------\n"; 
        std::cout << "encoded size: " << encoded.size()*sizeof(u32) << " (byte)" << std::endl; 
    }

    std::vector<char> decoded;

    if(LZ77Decode<char, u32>(encoded, decoded, indexBit, (u32) data.size())) {
        std::cout << "decoded: ";
        for(auto &e: decoded) {
            std::cout << e << " ";
        }
        std::cout << "\n";
    }
}