#include "bit.hpp"

template <class T>
int bitread(const std::vector<T> &B, int j) {
    if(j < 0) {
        std::cout << INDEX_NONNEGATIVE;
        return 0;  
    }
    if(B.size() == 0) return 0;
    int w = sizeof(B[0]) * 8;
    if(j >= w*B.size()) {
        std::cout << INDEX_OUTOFRANGE << j << "\n";
        return 0;
    }
    int r = j%w;
    return (B[j/w]>>(w-r-1))&1;
}

template<class T>
void bitset(std::vector<T> &B, int j) {
    if(j < 0) {
        std::cout << INDEX_NONNEGATIVE;
        return;  
    }
    if(B.size() == 0) return;
    int w = sizeof(B[0]) * 8;
    if(j >= w*B.size()) {
        std::cout << INDEX_OUTOFRANGE << j << "\n";
        return;
    }
    int r = j%w;
    if(((B[j/w]>>(w-r-1))&1) == 0) B[j/w] += 1<<(w-r-1);
}

template<class T>
void bitclear(std::vector<T> &B, int j) {
    if(j < 0) {
        std::cout << INDEX_NONNEGATIVE;
        return;  
    }
    if(B.size() == 0) return;
    int w = sizeof(B[0]) * 8;
    if(j >= w*B.size()) {
        std::cout << INDEX_OUTOFRANGE << j << "\n";
        return;
    }
    int r = j%w;
    if(((B[j/w]>>(w-r-1))&1) == 1) B[j/w] -= 1<<(w-r-1);
}

int main() {
    int a = 0, b=0;
    a += 1<<20; a+= 1<<13; a+=1<<5;
    b += 1<<31; b+= 1<<19; b+=1<<9;
    std::vector<int> v;
    v.emplace_back(a);
    v.emplace_back(b);
    for(int i=0;i<=63;i++) {
        if(bitread<int>(v, i) == 1) std::cout << 63-i << " ";
    }
    std::cout << "\n";

    bitset(v, 0);
    bitset(v, 8);
    for(int i=0;i<=63;i++) {
        if(bitread<int>(v, i) == 1) std::cout << 63-i << " ";
    } 
    std::cout << "\n";

    bitclear(v, 0);
    bitclear(v, 8);
    for(int i=0;i<=63;i++) {
        if(bitread<int>(v, i) == 1) std::cout << 63-i << " ";
    } 
    return 0;
}