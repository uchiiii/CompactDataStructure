#ifndef Huffman_hpp
#define Huffman_hpp

#include<unordered_map>
#include<queue>
#include<iostream>
#include<vector>
#include<string>

using namespace std;


struct Huffman {
    unordered_map<char, string> mp;
    void build(unordered_map<char,int> mp) {
        auto c = [](node *l, node *r) { return l->val > r->val;};
        priority_queue<node*, vector<node*>, decltype(c)> que(c);
    
        for(auto &[key, cnt]:mp) {
            que.push(new node(key, cnt));
        }
        while(que.size() != 1) {
            node* left = que.top(); que.pop();
            node* right = que.top(); que.pop();
            node* par = new node('$', left->val + right->val);
            par->left = left; par->right = right;
            que.push(par);
        }
        root = que.top(); que.pop();
        search(root); 
    }

    void build(vector<char> codes, vector<int> cnt) {
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
        root = que.top(); que.pop();
        search(root);
    }

    string encode(string s) {
        string ans = "";
        for(int i=0; i<s.size(); i++) {
            ans += mp[s[i]];
        }
        return ans;
    }

    string decode(string v) {
        node* cur = root;
        string ans = "";
        for(int i=0; i<v.size(); i++) {
            if(v[i]=='0') cur = cur->left;
            else cur = cur->right;
            if(cur->left == NULL and cur->right == NULL) {
                ans += cur->code;
                cur = root;
            }
        }
        return ans;
    }

private:
    struct node {
        int val;
        char code;
        node *left, *right;
        node(char c, int v) {
            val = v; code = c;
            left = right = NULL;
        }
    };
    node* root;
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

#endif
