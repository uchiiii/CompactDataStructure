#include<iostream>
#include<vector>

int main() {
    int n = (int) 'a';
    std::cout << n;
    for(int i=7; i>=0; i--) {
        std::cout << ((n>>i) & 1);
    }
    using u32 = unsigned int;
    std::cout << (std::numeric_limits<u32>::digits);
    std::cout << (std::numeric_limits<char>::digits); 
    return 0;
}