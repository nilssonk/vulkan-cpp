#ifndef OPTIONAL_MONAD_HH_INCLUDED_
#define OPTIONAL_MONAD_HH_INCLUDED_

namespace detail {

template<typename Input, typename... Fs>
struct final_result;

template<typename Input, typename F, typename... Fs>
struct final_result<Input, F, Fs...> {
    using output_ =
        typename std::remove_cv_t<std::invoke_result_t<F, Input>>::value_type;
    using type = typename final_result<output_, Fs...>::type;
};

template<typename Input, typename F>
struct final_result<Input, F> {
    using type = typename std::remove_cv_t<std::invoke_result_t<F, Input>>;
};

template<typename T, typename F>
constexpr auto
functional_do_impl(T && input, F && f)
{
    return f(std::forward<T>(input));
}

template<typename T, typename F, typename... Fs>
constexpr auto
functional_do_impl(T && input, F && f, Fs &&... fs) ->
    typename final_result<T, F, Fs...>::type
{
    auto result = f(std::forward<T>(input));
    if (!result.has_value()) {
        return std::nullopt;
    }

    return functional_do_impl(std::move(*result), std::forward<Fs>(fs)...);
}

} // namespace detail

template<typename T, typename... Fs>
[[nodiscard]] constexpr auto
functional_do(T && input, Fs &&... fs)
{
    return detail::functional_do_impl(std::forward<T>(input),
                                      std::forward<Fs>(fs)...);
}

#endif // OPTIONAL_MONAD_HH_INCLUDED_
