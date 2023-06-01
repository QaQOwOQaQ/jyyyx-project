#include <iostream>
#include <algorithm>
#include <string>
#include <cstddef>

using namespace std;

template<typename T1, typename T2>
inline auto max(T2 const& a, T1 const& b) -> decltype(a + b)
{
    return a < b ? b : a;
}

int main()
{
    cout << ::max(3, 3.13) << endl;
    return 0;
}