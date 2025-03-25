#include <iostream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>


using namespace std;

class Reverser 
{
public:
    template<typename Container>
    void operator()(Container& cont) 
    {
        std::reverse(cont.begin(), cont.end());
    }

    template<typename T, size_t N>
    void operator()(T (&arr)[N]) 
    {
        std::reverse(std::begin(arr), std::end(arr));
    }
};

template<typename T>
void print(const T& cont) 
{
    for (const auto& el : cont) 
    {
        std::cout << el << ' ';
    }
    std::cout << '\n';
}


int main() 
{
    Reverser rev;

    std::list<int> L = {1, 2, 3, 4, 5};
    rev(L);
    print(L);

    std::vector<int> v1 = {1, 2, 3, 4, 5};
    rev(v1);
    print(v1);

    std::string s1 = "abcdef";
    rev(s1);
    std::cout << s1 << '\n';

    int arr[] = {1, 2, 3, 4, 5};
    rev(arr);
    print(arr);

    return 0;
}
