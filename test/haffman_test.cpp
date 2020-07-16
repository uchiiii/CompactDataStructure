#include<vector>
#include<iostream>
#include "../src/Huffman.hpp"

int main() {
    vector<char> v = {'a', 'b', 'c', 'd', 'r'};
    vector<int> cnt = {5, 2, 1, 1, 2};
    Huffman hf;
    hf.build(v, cnt);
    for(auto &[k, v]: hf.mp) {
        cout << k << " " << v << endl;
    }

    string input = "ard";
    string encoded = hf.encode(input);
    cout << encoded << endl;

    cout << hf.decode(encoded) << endl;

    return 0;
}