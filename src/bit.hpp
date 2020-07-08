#ifndef __BIT_HPP_INCLUDED__
#define __BIT_HPP_INCLUDED__

#include<vector>
#include<iostream>
#include<string>

static const std::string INDEX_NONNEGATIVE= "bitclear index should be non-negative \n";
static const std::string INDEX_OUTOFRANGE = "index j is out of range, got ";

template <class T>
int bitread(const std::vector<T> &B, int j);

template<class T>
void bitset(std::vector<T> &B, int j);

template<class T>
void bitclear(std::vector<T> &B, int j);

#endif