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
    for(int i=0; i<sigma; i++) {
        cout << S[i] << " " << wt.access(i) << "\n";
    }

    // rank
    for(int i=0; i<sigma; i++) {
        cout << wt.rank(4, i) << "\n";
    }

    cout << "DEGBU FOR SELECT\n";
    for(int i=0; i<sigma; i++) {
        cout << wt.select(i, 1) << "\n";
    }
    // cout << wt.select(0, 1) << "\n";
    return 0;
}