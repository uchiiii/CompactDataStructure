#include<utility>
#include<vector>

using namespace std;

struct ZeroOrderBV {
    int b;
    vector<vector<int>> K;
    ZeroOrderBV(int b=32): b(b) {}
    pair<int, int> encode(int B) {
        int c = 0;
        for(int i=0;i<=b-1;i++) if(B>>i&1) c+=1;
        int o = 0;
        int c_cur = c;
        int j = 1;
        while(0 < c_cur and c_cur <= b-j) {
            if(bitread(B, j)==1) o += K[b-j][c_cur--];
            j++;
        }
        return {c, o};
    }
    int decode(pair<int, int> &p) {
        int B = 0;
        int j = 1;
        while(p.first > 0) {
            if(p.second >= K[b-j][p.first]) {
                bitset(B, j);
                p.second -= K[b-j][p.first--];
            }
            j++;
        }
        return B;
    } 
private:
    int bitread(const int &B, int j) {
        int w = sizeof(B) * 8;
        int r = j%w;
        return (B>>(w-r-1))&1;
    }

    void bitset(int &B, int j) {
        int w = sizeof(B) * 8;
        int r = j%w;
        if(((B>>(w-r-1))&1) == 0) B += 1<<(w-r-1);
    }
};