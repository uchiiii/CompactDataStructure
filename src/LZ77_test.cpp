#include "LZ77.hpp"
#include<iostream>
#include<vector>

int main() {
    std::vector<char> data = {'a', 'b', 'r', 'a', 'c', 'a', 'd', 'a', 'b', 'r', 'a'};
    std::vector<u32> encoded;
    SlidingWindowSearch<typename std::vector<char>::const_iterator, u32> swSearch;

    std::cout << "data size: " << data.size() * sizeof(char) << std::endl;

    if(LZ77Encode<char, u32, SlidingWindowSearch>(data, encoded, 16, swSearch)) {
        for(auto &e: encoded) {
            std::cout << e << " ";
        }
        std::cout << "\n";

        std::cout << "encoded size: " << encoded.size()*sizeof(u32) << std::endl; 
    }
}