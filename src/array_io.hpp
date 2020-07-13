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
void WriteToArray(const Data& data, Len length, Code& cachedCode, Len& cachedLen, std::vector<Code>& encData)
{
  W2A_PushOnce( data, length, cachedCode, cachedLen, encData );
  W2A_PushLoop( data, length, encData );
  W2A_SetLastCode( data, length, cachedCode, cachedLen);
}

template<class Data, typename Code, typename Len>
bool RFA_GetFromCache(typename std::vector<Code>::const_iterator& cit, const typename std::vector<Code>::const_iterator& decEnd, Code& cachedCode, Len& cachedLen, Data& data, Len& length)
{
    const Len szCode = std::numeric_limits<Code>::digits;
    if(cachedLen < length) { // when cached is not enough
        if(cit == decEnd) return false;
        // use cached code
        data = cachedCode & ~(~((Code)0) << cachedLen); 
        cachedCode = *(cit++);
        length -= cachedLen;
        cachedLen = szCode;
    } else { // when cached is enough
        cachedLen -= length; // now cachedLen is unnecessary part
        if(length == szCode) {
            data = cachedCode;
        } else {
            data = (cachedCode >> cachedLen) & ~(~((Code) 0) << length);
        }
        length = 0;
    }
    return true;
}

template<class Data,typename Code,typename Len>
bool RFA_GetLoop(typename std::vector<Code>::const_iterator& cit, const typename std::vector<Code>::const_iterator& decEnd, Code& cachedCode, Len& cachedLen, Data& data, Len& length)
{
    const Len szCode = std::numeric_limits<Code>::digits;

    while(length >= szCode) {
        data = (data << szCode) | cachedCode; // cachedCode is full.
        length -= szCode;
        if(length > 0 and cit==decEnd) return false;
        if(cit != decEnd) cachedCode = *(cit++);
    } 
    return true;
}

template<class Data,typename Code,typename Len>
void RFA_GetLastCode(Data& data, Len length, Code& cacheCode, Len& cacheLen)
{
    const Len szCode = std::numeric_limits<Code>::digits;
    if(length > 0) {
        data = (data << length) | (cacheCode >> (szCode-length));
        cacheLen -= length;
    }
}

/*
    Data class to accept both  T and Code.
*/
template<class Data,typename Code,typename Len> 
bool ReadFromArray(typename std::vector<Code>::const_iterator& cit, const typename std::vector<Code>::const_iterator& decEnd, Code& cachedCode, Len& cachedLen, Data& data, Len length)
{
    if(!RFA_GetFromCache(cit, decEnd, cachedCode, cachedLen, data, length)) return false;
    if(!RFA_GetLoop(cit, decEnd, cachedCode, cachedLen, data, length)) return false;
    RFA_GetLastCode(data, length, cachedCode, cachedLen);

    return true;
}

#endif