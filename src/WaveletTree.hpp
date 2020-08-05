#ifndef WaveletTree_hpp
#define WaveletTree_hpp

#include <algorithm>
#include <vector>
#include "bitvector.hpp"

using namespace std;

struct WaveletTree {

    /* data */
    vector<bitvector> T;
    int sigma;

    WaveletTree(vector<int> S, int _sigma): sigma(_sigma) { 
        int n = 1; while(n < sigma) n <<= 1;
        T.resize(2*n-1);
        build(S, 0, (int)S.size());
    }

    inline void build(vector<int> &S, const int a, const int b, int k=0, int l=0, int r=-1) {
        if(r<0) r = sigma;
        if(r-l==1) return;

        int mid = (l+r)/2;

        // bitvector initilization
        T[k].resize(b-a);
        for(int i=a; i<b; i++) {
            if(S[i] < mid) T[k].set(i-a, 0);
            else T[k].set(i-a,1);
        }
        T[k].build();

        auto it = stable_partition(S.begin()+a, S.begin()+b, [=] (int c) {
          return c < mid;
        });
        int z = distance(S.begin(), it); 

        build(S, a, z, 2*k+1, l, mid);
        build(S, z, b, 2*k+2, mid, r);
    }

    /* access to the value of S[i] */
    inline int access(int i, int k=0, int l=0, int r=-1) const {
        if(r<0) r = sigma;
        if(r-l==1) return l;
        if(T[k].access(i)==0) {
            return access(T[k].rank0(i), 2*k+1, l, (l+r)/2);
        } else {
            return access(T[k].rank1(i), 2*k+2, (l+r)/2, r);
        }
    }

    /* rank of c in range [0, i) */
    inline int rank(int c, int i, int k=0, int l=0, int r=-1) const {
        if(r<0) r = sigma;
        if(r-l==1) return i;
        if(c < (l+r)/2) {
            return rank(c, T[k].rank0(i), 2*k+1, l, (l+r)/2);
        } else {
            return rank(c, T[k].rank1(i), 2*k+2, (l+r)/2, r);
        }
    }

    /* return the position of jth c in S */
    inline int select(int c, int j) const {
        return select(c, j, 0, 0, sigma) - 1;
    }

    /* return the minimum i satisfying rank(c, i) = j */
    inline int select(int c, int j, int k, int l, int r) const {
        if(r-l==1) return j;
        if(c < (l+r)/2) {
            j = select(c, j, 2*k+1, l, (l+r)/2);
            if(j <= 0) return -1;
            return T[k].select0(j);
        } else {
            j = select(c, j, 2*k+2, (l+r)/2, r);
            if(j <= 0) return -1;
            return T[k].select1(j);
        }
    }
};

#endif