#pragma once

#include <array>
//#include <climits>
#include <limits>

// subarray() - copy a C-array, whole or a sub-range, as a std::array
//            - enables direct initialisation of std::array from C-array
//            - c.f. std::experimental::to_array(T[N]) -> std::array<T,N>
// e.g.
//      constexpr auto hello_6 = subarray("hello");
//      constexpr auto hello5 = subarray<0,-1>("hello");

// subarray<B=0,E=max()>(T[N]) -> std::array<T,b-e>
//
// Template arguments B & E are signed integers. Given array size N:
// non-negative values [ 0,  1,...,N-1, N) are offsets specified from begin,
//     negative values [-N,-N+1,...,-1]    are offsets specified from end
// and max() represents end itself.

namespace ltl {

constexpr long ind_max = std::numeric_limits<long>::max();

namespace impl {
// Array index from signed sub-index I, given the array size N, as above.
// *** Unchecked *** caller checks return value ret: 0 <= ret <= N
constexpr long ind(long I, unsigned long N)
{
    return I==ind_max ? N : I<0 ? N+I : I;
}

} // namespace impl

// subarray<b,e>(T[N]) -> std::array<T,e-b>
template <long B=0, long E=ind_max, typename T, long N,
          long b=impl::ind(B,N), long e=impl::ind(E,N)>
requires 0<=b && b<=e && e<=N
constexpr
auto
subarray(T const (&a)[N])
{
    std::array<T,e-b> ret{};
    for (size_t i=0; i<e-b; ++i)
        ret[i] = a[b+i];
    return ret;
}

// sget<I>(T[N]) array sub-indexed get(), i.e. with signed index
template <long I, typename T, size_t N>
requires 0 <= impl::ind(I,N) &&
              impl::ind(I,N) < N
constexpr
T
sget(T const (&a)[N])
{
    return a[impl::ind(I,N)];
}

template <long I, typename T, size_t N>
requires 0 <= impl::ind(I,N) &&
              impl::ind(I,N) < N
constexpr
T
sget(std::array<T,N> const& a)
{
    return a[impl::ind(I,N)];
}

} // namespace ltl
