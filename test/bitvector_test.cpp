#include <iostream>
#include <vector>

#include "../src/bitvector.hpp"

int main () {

    bitvector bv;
    int sz = 10;

    bv.resize(sz);
    bv.set(1,1); bv.set(2,1);
    bv.build();

    cout << "DEBUG FOR RANK\n";
    for(int i=1; i<5; i++) cout << bv.rank0(i) << endl;
    for(int i=1; i<5; i++) cout << bv.rank1(i) << endl;

    cout << "DEBUG FOR SELECT\n";
    cout << bv.select0(2) << endl; // 4
    cout << bv.select1(1) << endl; // 2
    return 0;   
}