#ifndef UTIL_GUARD
#define UTIL_GUARD

#include<cstddef> 

#include "type_traits.h"

namespace mystl
{

// move
// cast from left/right value to right value
template<class T>
typename std::remove_reference<T>::type&& move(T &&arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

// forward
// left  value -> pass by left value
template<class T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept 
{
    return static_cast<T&&>(arg);
}
// right value -> pass by right value
template<class T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    return static_cast<T&&>(arg);
}

// swap
template<class T>
void swap(T& lhs, T& rhs)
{
    auto tmp(mystl::move(lhs));
    lhs = mystl::move(rhs);
    rhs = mystl::move(tmp);
}

// helper function
// return end ForwardIter2's end iterator
template<class ForwardIter1, class ForwardIter2>
ForwardIter2 swap_range(ForwardIter1 first1, ForwardIter1 last1, ForwardIter2 first2)
{
    for(; first1 != last1; ++ first1, ++ first2)
        mystl::swap(*first1, *first2);
    return first2;
}

template<class T, size_t N>
void swap(T(&a)[N], T(&b)[N])
{
    mystl::swap_range(a, a + N, b);
}

/*************************************************************/

// pair
template<class T1, class T2>
struct pair
{
    typedef T1      first_type;
    typedef T2      second_type;

    first_type first;
    second_type second;

    // default constructiable
    template<class Other1 = T1, class Other2 = T2,
        typename = typename std::enable_if< 
            std::is_default_constructible<Other1>::value && 
            std::is_default_constructible<Other2>::value, void>::type>
        constexpr pair()
        : first(), second() {}

};

} //namespace mystl



#endif 