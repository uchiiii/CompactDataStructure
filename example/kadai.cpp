#include <iostream>
#include <fstream>
#include <unistd.h>
#include "../src/LZ77.hpp"
#include "../src/Huffman.hpp"
#include "../src/LZ78.hpp"


int main() {
    using namespace std;

    char dir[255];
    getcwd(dir,255);
    cout << "Current Directory : " << dir << endl;

    ifstream ifs("./example/data/modelB.txt", ios::in);

    if(!ifs) {
        cerr << "Error: file not opened." << endl;
        return 1;
    }

    unsigned int num;
    string input;
    ifs >> num >> input;

    ////////////////// Huffman Code 1 /////////////////////
    std::cout << "Huffman Code 1\n";
    
    vector<char> v_1 = {'A', 'T', 'G', 'C'};
    vector<int> cnt_1 = {4, 2, 1, 1}; // here is different.
    Huffman hf_1;
    hf_1.build(v_1, cnt_1);
    for(auto &[k, v]: hf_1.mp) {
        cout << k << " " << v << endl;
    }

    string hf_1_encoded = hf_1.encode(input);
    cout << hf_1_encoded << endl;

    std::cout << "Huffman encoded size: " <<  (hf_1_encoded.size() + 7) / 8 << " (byte)" << std::endl;

    cout << hf_1.decode(hf_1_encoded) << endl;

    std::cout << "\n\n//////////////////////////////////////////////////////////\n\n";

    ////////////////// Huffman Code 2 /////////////////////
    std::cout << "Huffman Code 2\n";
    
    vector<char> v_2 = {'A', 'T', 'G', 'C'};
    vector<int> cnt_2 = {2, 2, 2, 2}; // here is different.
    Huffman hf_2;
    hf_2.build(v_2, cnt_2);
    for(auto &[k, v]: hf_2.mp) {
        cout << k << " " << v << endl;
    }

    string hf_2_encoded = hf_2.encode(input);
    cout << hf_2_encoded << endl;

    std::cout << "Huffman encoded size: " <<  (hf_2_encoded.size() + 7) / 8 << " (byte)" << std::endl;

    cout << hf_2.decode(hf_2_encoded) << endl;

    std::cout << "\n\n//////////////////////////////////////////////////////////\n\n";

    ////////////////// Huffman Code 3 ///////////////////// 
    std::cout << "Huffman Code 3\n";
    
    vector<char> v_3 = {'A', 'T', 'G', 'C'};
    vector<int> cnt_3 = {1, 1, 2, 4}; // here is different.

    Huffman hf_3;
    hf_3.build(v_3, cnt_3);
    for(auto &[k, v]: hf_3.mp) {
        cout << k << " " << v << endl;
    }

    string hf_3_encoded = hf_3.encode(input);
    cout << hf_3_encoded << endl;

    std::cout << "Huffman encoded size: " <<  (hf_3_encoded.size() + 7) / 8 << " (byte)" << std::endl;

    cout << hf_3.decode(hf_3_encoded) << endl;

    std::cout << "\n\n//////////////////////////////////////////////////////////\n\n";

    ////////////////// LZ77 /////////////////////
    std::cout << "LZ77\n";

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
        std::cout << "LZ77 encoded size: " << encoded.size()*sizeof(u32) << " (byte)" << std::endl; 
    }

    std::vector<char> decoded;

    if(LZ77Decode<char, u32>(encoded, decoded, indexBit, (u32) data.size())) {
        std::cout << "decoded: ";
        for(auto &e: decoded) {
            std::cout << e;
        }
        std::cout << "\n";
    }

    std::cout << "\n\n//////////////////////////////////////////////////////////\n\n";
    
    ////////////////// LZW /////////////////////
    std::cout << "LZW\n";
    std::vector<Code> lzw_encoded;

    vector<unsigned char> lzw_data;
    for(auto e: input) {
        lzw_data.emplace_back((unsigned char) e);
    }
    std::cout << "input data size: " << lzw_data.size() * sizeof(u8) << " (byte)" << std::endl;

    if(LZWEncode(lzw_data, lzw_encoded)) {
        std::cout << "--------------------------------------------------------------\n";
        for(auto &e: lzw_encoded) {
            std::cout << e << " ";
            for(int i=31; i>=0; i--) {
                std::cout << ((e >> i) & 1);
            }
            std::cout << "\n";
        }

        std::cout << "--------------------------------------------------------------\n"; 
        std::cout << "encoded size: " << lzw_encoded.size()*sizeof(Code) << " (byte)" << std::endl; 
    }
    return 0;
}