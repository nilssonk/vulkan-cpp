#ifndef TO_ARRAY_HH_INCLUDED_
#define TO_ARRAY_HH_INCLUDED_

#include <array>
#include <utility>

namespace detail {

template<class T, std::size_t N, std::size_t... I>
// NOLINTNEXTLINE
constexpr auto
to_array_impl(T(&&a)[N], std::index_sequence<I...>)
    -> std::array<std::remove_cv_t<T>, N>
{
    return {{std::forward<decltype(a[I])>(a[I])...}};
}

} // namespace detail

template<class T, std::size_t N>
// NOLINTNEXTLINE
constexpr auto
to_array(T(&&a)[N]) -> std::array<std::remove_cv_t<T>, N>
{
    return detail::to_array_impl(std::forward<decltype(a)>(a),
                                 std::make_index_sequence<N>{});
}

#endif // TO_ARRAY_HH_INCLUDED_
