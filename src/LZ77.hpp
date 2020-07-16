#ifndef LZ77_hpp
#define LZ77_hpp

#include<iostream>
#include<vector>
#include "array_io.hpp"

using u32 = unsigned int;

template<class Code>
struct LZ77_CodeGenerator {
    u32 indexBit_;
    u32 countBit_;

    Code indexMask_;
    Code countMask_;

    LZ77_CodeGenerator(u32 indexBit, u32 countBit): indexBit_(indexBit), countBit_(countBit) {
        indexMask_ = ~(~((Code)0) << indexBit_);
        countMask_ = ~(~((Code)0) << countBit_);
    }

    Code encode(Code index, Code count) const {
        return (index << countBit_) | count;
    }

    void decode(Code code, Code& index, Code& count) const {
        index = (code >> countBit_) & indexMask_;
        count = (code & countMask_);
    }

    u32 bits() const {
        return indexBit_ + countBit_;
    }
};

template<class In, class Code>
struct SlidingWindowSearch {
    void operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code& index, Code& count);
};

template<class In, class Code>
void SlidingWindowSearch<In, Code>::operator()(In dataStart, In dataEnd, Code winSize, Code maxCountSize, Code& index, Code& count)
{
    using difference_type = typename std::iterator_traits<In>::difference_type;

    index = count = Code();
    
    In winStart = dataStart;
    std::advance(winStart, -static_cast<difference_type>(winSize));

    for(;winStart!=dataStart; ++winStart) {
        In winP = winStart;

        for(In dataP = dataStart; *winP==*dataP and winP != dataStart; dataP++) { // winP should not get over dataStart
            if(dataP == dataEnd) break;
            ++winP;
        }

        u32 sz = std::distance(winStart, winP);
        if(sz > count) {
            index = std::distance(winStart, dataStart);
            count = sz;
        }

        if(count >= maxCountSize) {
            count = maxCountSize;
            break;
        }
    }    
}

template<class In, class Code>
struct SlidingWindow {

    using value_type = typename std::iterator_traits<In>::value_type;

    In dataStart_;
    In dataEnd_;
    In curIndex_;

    Code winSize_;
    Code maxWinSize_;
    Code maxCountSize_;

    SlidingWindow(In s, In e, Code maxWinSize, Code maxCountSize)
    : dataStart_(s), dataEnd_(e), curIndex_(s), winSize_(0), maxWinSize_(maxWinSize), maxCountSize_(maxCountSize)
    {
        if(std::distance(s, e) < maxWinSize_) maxWinSize_ = distance(s, e);
    }

    template<class Container> 
    SlidingWindow(const Container& container, Code maxWinSize, Code maxCountSize)
    : dataStart_(container.begin()), dataEnd_(container.end()), curIndex_(container.begin()), winSize_(0), maxWinSize_(maxWinSize), maxCountSize_(maxCountSize)
    {}

    template< template<class, class> class SlidingWindowSearch>
    void search(SlidingWindowSearch<In, Code>& search, Code& index, Code& count) const {
        search(curIndex_, dataEnd_, winSize_, maxCountSize_, index, count);
    }

    value_type value() const {
        return *curIndex_;
    }

    bool empty() const {
        return curIndex_ == dataEnd_;
    }

    void increment(u32 n);
};

template<class In, class Code> 
void SlidingWindow<In, Code>::increment(u32 n) {
    if((u32)std::distance(curIndex_, dataEnd_) < n) {
        curIndex_ = dataEnd_;
    } else {
        curIndex_ += n;
    }

    if(winSize_ < maxWinSize_) { // to keep winSize_ less than or equal to the max.
        winSize_ = std::min(winSize_+n, maxWinSize_);
    }
}

/*
    indexBit: size of digits for index
    T: data type, 
*/
template<class T, class Code, template<class, class> class SlidingWindowSearchType>
bool LZ77Encode(const std::vector<T>& data, std::vector<Code>& packed, u32 indexBit, SlidingWindowSearchType<typename std::vector<T>::const_iterator, Code>& swSearch)
{
    const u32 szCode = sizeof(Code) / sizeof(char); // size of Code (byte)
    const u32 bitCode = std::numeric_limits<Code>::digits; // the digits of Code without sign
    const u32 szValue = sizeof(T) / sizeof(char); // size of Data type(T) (byte)
    const u32 bitValue = std::numeric_limits<T>::digits;

    if(data.size() == 0) return false;

    if(indexBit >= bitCode) return false;

    u32 countBit = bitCode - indexBit;
    Code maxWinSize = (1 << indexBit) - 1;
    Code maxCountSize = (1 << countBit) - 1;

    LZ77_CodeGenerator<Code> codeGen(indexBit, countBit);
    SlidingWindow<typename std::vector<T>::const_iterator,Code> sWin(data, maxWinSize, maxCountSize);
    
    Code cachedCode = 0;
    u32 cachedLen = 0;
    Code index = 0;
    Code count = 0;

    packed.clear();
    while(!sWin.empty()) {
        sWin.search(swSearch, index, count);
        // std::cout << "index:" << index << std::endl;
        // std::cout << "count:" << count << std::endl;

        if(count * szValue > szCode) {
            WriteToArray((Code)1, (u32)1, cachedCode, cachedLen, packed);
            WriteToArray(codeGen.encode(index, count), codeGen.bits(), cachedCode, cachedLen, packed);
            sWin.increment(count);
        } else {
            WriteToArray((Code)0, (u32)1, cachedCode, cachedLen, packed);
            WriteToArray(sWin.value(), bitValue, cachedCode, cachedLen, packed);
            sWin.increment(1);
        }
        // std::cout << "cachedLen:" << cachedLen << std::endl;
    }

    if(cachedLen > 0) //  上のelseの時に残った分を埋める.
    packed.push_back(cachedCode << (bitCode - cachedLen));
     
    return true;
}

template<class T, class Code>
bool LZ77Decode(const std::vector<Code>& packed, std::vector<T>& data, u32 indexBit, typename std::vector<T>::size_type dataCount)
{
    const u32 szCode = sizeof(Code) / sizeof(char);
    const u32 bitCode = std::numeric_limits<Code>::digits;
    const u32 bitValue = std::numeric_limits<T>::digits;
    
    if(packed.size() == 0) return false;

    if(indexBit >= bitCode) return false;

    u32 countBit = bitCode - indexBit;

    Code cachedCode = 0;
    u32 cachedLen = 0;
    Code code;
    Code index = 0;
    Code count = 0;
    T isCode;
    T t; 

    LZ77_CodeGenerator<Code> codeGen(indexBit, countBit);

    auto cit = packed.begin();
    typename std::vector<T>::size_type st = 0; // # of decoded data

    while(st < dataCount) {
        if(!ReadFromArray<T, Code, u32>(cit, packed.end(), cachedCode, cachedLen, isCode, (u32) 1)) return false;
        // std::cout << "isCode:" << (int) isCode << std::endl;
        if(isCode != 0) {
            // std::cout << codeGen.bits();
            if(!ReadFromArray<Code, Code, u32>(cit, packed.end(), cachedCode, cachedLen, code, codeGen.bits())) return false;
            codeGen.decode(code, index, count);
            typename std::vector<T>::size_type read_pos = data.size() - index;
            
            for(u32 sz = 0; sz < count; sz++) { 
                data.push_back(data[read_pos]);
                read_pos++;
            }

            st += count;
        } else {
            if(!ReadFromArray<T, Code, u32>(cit, packed.end(), cachedCode, cachedLen, t, (u32) bitValue)) return false;
            data.push_back(t);
            st++;
        }
    }

    return true;
}

#endif