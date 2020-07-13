#ifndef Trie_hpp
#define Trie_hpp

#include<vector>

template<typename T>
struct Trie {
    using array_type = typename std::vector<Node>;
    using size_type = typename array_type::size_type;
    using u32 = unsigned int;
    struct Node{
        T value;
        size_type parent;
        size_type bros;
        size_type child;
        u32 level;        
    
        Node(T t, size_type p, u32 lev): T(t), parent(p), level(lev) {}
    };

    array_type nodes;

    size_type nullNode() {
        return nodes.max_size();
    }

    static size_type rootNode() {
        return 0;
    }

    Trie() { init(); }
    void init() {
        nodes.clear();
        add(nullNode, )
    }
};

#endif