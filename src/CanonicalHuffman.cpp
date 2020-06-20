#include<vector>
#include<iostream>
#include<algorithm>
#include<queue>

using namespace std;

struct CanonicalHaffman {
    struct node {
        int val;
        char code;
        node *left, *right;
        node(char c, int v) {
            val = v; code = c;
            left = right = NULL;
        }
    };
    int h; // maximum length of the encoded
    vector<char> L;
    vector<int> F; // index
    vector<int> C; // encoded
    void build(vector<char> codes, vector<int> cnt) {
        if(codes.size() != cnt.size() or codes.size() == 0) {
            cout << "invalid input" << "\n";
            return;
        }
        node *root = build_haffman_tree(codes, cnt);
        vector<pair<int, char>> s1;
        compute_length(root, 0, s1);
        auto s2 = create_code(s1);
        for(int i=0; i<s2.size(); i++) {
            L.emplace_back(get<0>(s2[i]));
        }
        h = get<2>(s2[s2.size()-1]);
        C.resize(h+2);
        C[h+1] = 2 << (h+1); // for binary search
        for(int i=0; i<=h; i++) {
            F.emplace_back(0);
        }
        for(int i=s2.size()-1; i>=1; i--) {
            F[get<2>(s2[i])] = i;
            C[get<2>(s2[i])] = get<1>(s2[i]);
        }
        for(int i=h-1; i>=2; i--) {
            if(F[i] == 0) {
                F[i] = F[i+1];
                C[i] = C[i+1]>>1;
            }
        }
    }
    string decode(string s) {
        int left, plus;
        left=0; plus=1;
        const int MX_BINARY_SEARCH = floor_log(h);
        string ans = "";
        while(1) {
            if(left == s.size()) return ans;
            while(left+plus<=s.size() and plus < h) plus++; // [l, l+plus)
            string cur = "";
            // cout << left << " " << left+plus << "\n";
            for(int i=0; i<plus; i++) cur += s[left+i];
            // cout << "cur:" << cur << "\n";
            int N = binary_str_2_int(cur);
            // cout << "N:" << N << "\n";

            // binary search
            int l = 1;
            for(int i=MX_BINARY_SEARCH; i>=0; i--) {
                int l_cur = l + (1<<i);
                if(l_cur > h) continue;
                if(C[l_cur+1]<<(h-l_cur-1) <= N) l = l_cur;
                else if(C[l_cur]<<(h-l_cur) <= N and N < C[l_cur+1]<<(h-l_cur-1)) {
                    l = l_cur;
                    break;
                }
            }
            cout << l << "\n";
            N >>=(h-l);
            ans += L[F[l]+N-C[l]];
            left += l;
            plus = 1;
        }
        return ans;
    } 
    node* build_haffman_tree(vector<char> codes, vector<int> cnt) {
        auto c = [](node *l, node *r) { return l->val > r->val;};
        priority_queue<node*, vector<node*>, decltype(c)> que(c);
        
        for(int i = 0; i <= (int)codes.size()-1; i++) {
            que.push(new node(codes[i], cnt[i]));
        }
        while(que.size() != 1) {
            node* left = que.top(); que.pop();
            node* right = que.top(); que.pop();
            node* par = new node('$', left->val + right->val);
            par->left = left; par->right = right;
            que.push(par);
        }
        return que.top();
    }
    // tuple.first is a code
    // tuple.second is a encoded
    vector<tuple<char, int, int>> create_code(vector<pair<int, char>> v) {
        vector<tuple<char, int, int>> ans;
        sort(v.begin(), v.end());
        ans.emplace_back(v[0].second, 0, 1);
        int cur = 0;
        for(int i=1; i<v.size(); i++) {
            cur += 1; cur<<=(v[i].first-v[i-1].first);
            ans.emplace_back(v[i].second, cur, v[i].first);
        }
        return ans;
    }
    void compute_length(node* nd, int len, vector<pair<int, char>> &v) {
        if(nd->left == NULL and nd->right == NULL) {
            v.emplace_back(len, nd->code);
            return;
        }
        if(nd->left != NULL) compute_length(nd->left, len+1, v);
        if(nd->right != NULL) compute_length(nd->right, len+1, v);
    }
    int binary_str_2_int(string s) {
        int ans = 0;
        for(int i=0; i<s.size(); i++) {
            ans <<= 1;
            if(s[i] == '1') ans += 1;
        }
        return ans;
    }
    int floor_log(int k) {
        int ans = 0;
        int cur = 1;
        while(cur <= k) ans += 1, cur <<= 1;
        return ans;
    }
};

int main() {
    CanonicalHaffman cf;
    vector<pair<int, char>> v1 = {{1, 'a'}, {4, 'c'}, {4, 'd'}, {3, 'b'}, {2, 'r'}};
    // for(auto [a,b,c] : cf.create_code(v1)){
    //     cout << a << " " << b << " " << c << '\n';
    // }

    vector<char> v2 = {'a', 'b', 'c', 'd', 'r'};
    vector<int> cnt = {5, 2, 1, 1, 2};

    cf.build(v2, cnt);
    cout << cf.decode("1100101110") << "\n"; // "rabc"
}
