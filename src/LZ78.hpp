#ifndef LZ78_hpp
#define LZ78_hpp

#include "array_io.hpp"
#include "Trie.hpp"
#include<vector>

using Code = Trie<unsigned char>::size_type;
using u8 = unsigned char;

void SetDefaultNode(Trie<u8>& trie) {
    trie.init();

    Code bigBros = trie.add(trie.rootNode(), 0);
    trie.setChild(trie.rootNode(), bigBros);

    for(unsigned int i=1; i<=std::numeric_limits<u8>::max(); i++) {
        Code littleBros = trie.add(trie.rootNode(), i);
        trie.setBros(bigBros, littleBros);
        bigBros = littleBros;
    }
}

Code GetBitSize(Code index) {
    Code maxBitSize = std::numeric_limits<Code>::digits;

    Code res = 0;
    for(Code i=0; i<maxBitSize; i++) {
        if(((index >> i)&1)==1) res = i;
    }
    return ++res;
}

bool LZWEncode(const std::vector<u8>& data, std::vector<Code>& packed) {
    Trie<u8> dict;
    SetDefaultNode(dict);

    Code dicIdx = dict.rootNode();
    Code maxDicIdx = dicIdx;

    std::vector<Code> buffer;

    auto src = data.begin();
    while(src != data.end()) {
        Code childIdx = dict.findChild(dicIdx, *src);
        if(childIdx != dict.nullNode()) {
            dicIdx = childIdx;
            ++src;
        } else {
            --dicIdx; // trieのrootの0を除く. 結局, decodeでは++することが必要.
            buffer.push_back(dicIdx);
            if(dicIdx > maxDicIdx) maxDicIdx = dicIdx; // 符号の最大値チェック
            dicIdx = dict.rootNode();
        }
    }

    if(dicIdx != dict.nullNode()) {
        --dicIdx;
        if(dicIdx > maxDicIdx) maxDicIdx = dicIdx;
        buffer.push_back(dicIdx);
    }

    Code bitSize = GetBitSize(maxDicIdx);
    Code cachedCode = 0;
    Code cachedLen = 0;
    
    packed.clear();
    packed.push_back(bitSize);
    for(auto cit = buffer.begin(); cit != buffer.end(); cit++) {
        WriteToArray(*cit, bitSize, cachedCode, cachedLen, packed);
    }

    if(cachedLen > 0) {
        packed.push_back(cachedCode << (std::numeric_limits<Code>::digits - cachedLen));
    }

    return true;
}


#endif