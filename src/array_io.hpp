#ifndef Array_io_hpp
#define Array_io_hpp

#include<vector>

template<class Data,typename Code,typename Len>
void W2A_PushOnce(const Data& data, Len& length, Code& cachedCode, Len& cachedLen, std::vector<Code>& encData)
{
    const Len szCode = std::numeric_limits<Code>::digits;
    Len remained = szCode - cachedLen;

    if(remained <= length) {
        if(remained == szCode) encData.push_back(data >> (length-remained)); // when cachedLen = 0;
        else encData.push_back((cachedCode << remained) | (data >> (length-remained)));

        cachedLen = 0;
        length -= remained;
    } else {
        cachedLen += length;
        cachedCode = (cachedCode << length) | data;
        length = 0;
    }
}

template<class Data,typename Code,typename Len>
void W2A_PushLoop(const Data& data, Len& length, std::vector<Code>& encData)
{
    const Len szCode = std::numeric_limits<Code>::digits;

    while(length >= szCode) {
        encData.push_back((data >> (length-szCode)) & ~((Code)0));
        length -= szCode;
    }
}

template<class Data,typename Code,typename Len>
void W2A_SetLastCode(const Data& data, Len& length, Code& cachedCode, Len& cachedLen) {
    if(length > 0) {
        cachedCode = data & ~(~((Code)0) << length);
        cachedLen = length;
        length = 0;
    }
}

template<class Data,typename Code,typename Len>
void WriteToArray
( const Data& data, Len length, Code& cachedCode, Len& cachedLen, std::vector<Code>& encData)
{
  W2A_PushOnce( data, length, cachedCode, cachedLen, encData );
  W2A_PushLoop( data, length, encData );
  W2A_SetLastCode( data, length, cachedCode, cachedLen);
}

#endif