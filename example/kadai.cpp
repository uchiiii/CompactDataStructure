#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../src/LZ77.hpp"


int main() {
    using namespace std;

    char dir[255];
    getcwd(dir,255);
    cout << "Current Directory : " << dir << endl;

    ifstream ifs("./example/data/modelA.txt", ios::in);

    if(!ifs) {
        cerr << "Error: file not opened." << endl;
        return 1;
    }

    unsigned int num;
    string input;
    ifs >> num >> input;

    vector<char> data;
    for(auto e: input) {
        data.emplace_back(e);
    }

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

    return 0;
}