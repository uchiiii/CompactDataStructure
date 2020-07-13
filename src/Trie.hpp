#ifndef Trie_hpp
#define Trie_hpp

#include<vector>

template<typename T>
struct Trie {
    struct Node;
    using array_type = typename std::vector<Node>;
    using size_type = typename array_type::size_type;
    using u32 = unsigned int;
    struct Node{
        T value;
        size_type parent;
        size_type bros;
        size_type child;
        u32 level;        
    
        Node(T t, size_type p, u32 lev): value(t), parent(p), level(lev) {}
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
        add(nullNode(), 0);
        setChild(rootNode(), nullNode());
        setBros(rootNode(), nullNode());
    }

    size_type size() const {
        return nodes.size();
    }

    size_type add(size_type parent, T newval);

    size_type findChild(size_type index, T val);

    size_type findBros(size_type index, T val);

    T operator[](size_type index) const {
        return nodes.at(index).value;
    }

    size_type parent(size_type index) const {
        return nodes.at(index).parent;
    }

    u32 level(size_type index) const {
        return nodes.at(index).level;
    }

    void setChild(size_type parent, size_type child) {
        nodes.at(parent).child = child;
    }

    void setBros(size_type bigBros, size_type littleBros) {
        nodes.at(bigBros).bros = littleBros;
    }
};

template<typename T>
typename Trie<T>::size_type Trie<T>::add(size_type parent, T newVal) {
    nodes.push_back(Node(newVal, parent, (parent==nullNode())? 0: nodes.at(parent).level+1));
    nodes.back().child = nullNode();
    nodes.back().bros = nullNode();

    return nodes.size() - 1;
}

template<typename T>
typename Trie<T>::size_type Trie<T>::findChild(size_type index, T val) {
    if(nodes.at(index).child == nullNode()) {
        setChild(index, add(index, val));
        return nullNode();
    } else {
        return findBros(nodes.at(index).child, val);
    }
}

template<typename T>
typename Trie<T>::size_type Trie<T>::findBros(size_type index, T val) {
    for(; nodes.at(index).value != val; index = nodes.at(index).bros) {
        if(nodes.at(index).bros == nullNode()) {
            setBros(index, add(nodes.at(index).parent, val));
            return nullNode();
        }
    }
    return index;
}

#endif