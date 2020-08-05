#ifndef bitvector_hpp
#define bitvector_hpp

#include<vector>
#include<iostream>

using namespace std;

using u8 = unsigned char;
using u16 = unsigned short;

// This supports n <= 2^{16}, simply because it is easy to implement.
struct bitvector {
    u16 length, cnum, bnum;

    static int cwidth, bwidth; // (256, 8)

    vector<u8> bit;
    vector<u16> chunks;
    vector<vector<u8>> blocks;
    static vector<u8> table;

    bitvector() {}

    void resize(int _n) {
        length = _n; cnum = (_n+cwidth-1)/cwidth; bnum = cwidth / bwidth;
        bit.assign(cnum*bnum, 0);
        chunks.assign(cnum+1, 0);
        blocks.assign(cnum, vector<u8>(bnum, 0));
    }

    void set(int pos, int b=1) {
        if(pos >= length) {
            cout << "invalid position. \n";
        }
        int bitpos = pos/bwidth;
        int offset = pos%bwidth;

        if(b == 0) bit[bitpos] &= ~(1<<offset);
        else if(b == 1) bit[bitpos] |= (1<<offset);
        else cout << "invalid b value. \n";
    }

    int access(int pos) const {
        if(pos >= length) {
            cout << "invalid position. \n";
        }
        int bitpos = pos/bwidth;
        int offset = pos%bwidth;
        return (bit[bitpos] >> offset) & 1;
    }

    u8 popCount(u8 c) const {
        return __builtin_popcount(c);
        // if you cannot use above, you can use the table below.
        // return table[c];
    }

    void build() {
        chunks[0] = 0;
        for(int i=0; i<cnum; i++) {
            blocks[i][0] = 0;
            for(int j=1; j<bnum; j++) {
                blocks[i][j] = blocks[i][j-1] + popCount(bit[i*bnum+j-1]);
            }
            chunks[i+1] = chunks[i] + blocks[i][bnum-1] + popCount(bit[(i+1)*bnum-1]);
        }
    }

    // rank1 in range [0, pos)
    int rank1(int pos) const {
        int cpos = pos / cwidth;
        int bpos = pos % cwidth / bwidth;
        int offset = pos % bwidth;

        u8 masked = (bit[cpos*bnum+bpos]) & ((1<<offset)-1);
        return chunks[cpos] + blocks[cpos][bpos] + popCount(masked);
    }

    int rank0(int pos) const {
        return pos - rank1(pos);
    }

    // return the minimum i satisfying rank(i) = num
    int select1(int num) const {
        if(num == 0) return 0;
        if(rank1(length) < num) return -1;

        int left = 0, right = length;
        while(left + 1 < right) {
            int mid = (left+right)/2;
            if(rank1(mid) >= num) right = mid;
            else left = mid;
        }
        return right; 
    }

    int select0(int num) const {
        if(num == 0) return 0;
        if(rank0(length) < num) return -1;

        int left = 0, right = length;
        while(right - left > 1) {
            int mid = (left+right)/2;
            if(rank0(mid) >= num) right = mid;
            else left = mid;
        }
        return right;
    }
};

int bitvector::cwidth = (1<<8);
int bitvector::bwidth = (1<<3);
// vector<u8> bitvector::table = {
//     0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 
//     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
//     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
//     1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
//     2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6, 
//     3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
//     3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7, 
//     4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8, 
// };

#endif 