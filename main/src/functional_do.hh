#ifndef FUNCTIONAL_DO_HH_INCLUDED_
#define FUNCTIONAL_DO_HH_INCLUDED_

#include "monad.hh"

namespace detail {

template<typename T, typename F>
constexpr auto
functional_do_impl(T && input, F && f)
{
    return Monad<T>::bind(std::forward<T>(input), std::forward<F>(f));
}

template<typename T, typename F, typename... Fs>
constexpr auto
functional_do_impl(T && input, F && f, Fs &&... fs)
{
    auto result = Monad<T>::bind(std::forward<T>(input), std::forward<F>(f));

    return functional_do_impl(std::move(result), std::forward<Fs>(fs)...);
}

} // namespace detail

template<typename T, typename... Fs>
constexpr auto
functional_do(T && input, Fs &&... fs)
{
    return detail::functional_do_impl(std::forward<T>(input),
                                      std::forward<Fs>(fs)...);
}

#endif // FUNCTIONAL_DO_HH_INCLUDED_
