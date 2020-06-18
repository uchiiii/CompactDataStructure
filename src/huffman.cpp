#include<unordered_map>
#include<queue>
#include<iostream>
#include<vector>
#include<string>

using namespace std;


template<class T>
struct Huffman {
    struct node {
        int val;
        T code;
        node *left, *right;
        node(T c, int v) {
            val = v; code = c;
            left = right = NULL;
        }
    };
    // vector<T> codes;
    // vector<int> cnt;
    unordered_map<T, string> mp;
    // Huffman(vector<T> v, vector<int> c): codes(v), cnt(c){}
    void encode(vector<T> codes, vector<int> cnt) {
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
        node* top = que.top(); que.pop();
        search(top);
    }

private:
    void search(node* nd, string s="") {
        if(nd->left == NULL and nd->right == NULL) {
            mp[nd->code] = s;
            return;
        }
        if(nd->left != NULL) {
            search(nd->left, s + "0");
        }
        if(nd->right != NULL){
            search(nd->right, s + "1");
        } 
    }
};

int main() {
    vector<char> v = {'a', 'b', 'c', 'd', 'r'};
    vector<int> cnt = {5, 2, 1, 1, 2};
    Huffman<char> hf;
    hf.encode(v, cnt);
    for(auto [k, v]: hf.mp) {
        cout << k << " " << v << endl;
    }

    return 0;
}
