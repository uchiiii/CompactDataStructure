#include <iostream>
#include <vector>

#include "../src/WaveletTree.hpp"

int main () {

    vector<int> S; 
    int sigma = 10;
    for(int i=0;i<sigma;i++) {
        S.push_back(i);
    }
    WaveletTree wt(S, sigma);
    
    // access
    cout << "DEBUG FOR ACCESS\n";
    for(int i=0; i<sigma; i++) {
        cout << S[i] << " " << wt.access(i) << "\n";
    }

    // rank
    cout << "DEBUG FOR RANK\n"; 
    for(int i=0; i<sigma; i++) {
        cout << wt.rank(4, i) << "\n";
    }

    cout << "DEGBU FOR SELECT\n";
    for(int i=0; i<sigma; i++) {
        cout << wt.select(i, 1) << "\n";
    }

    cout << wt.select(3, 3) << "\n"; // return negative when something wrong.
    return 0;
}