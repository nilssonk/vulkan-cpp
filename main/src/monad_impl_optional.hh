#ifndef MONAD_IMPL_OPTIONAL_HH_INCLUDED_
#define MONAD_IMPL_OPTIONAL_HH_INCLUDED_

#include "monad.hh"

#include <optional>

template<typename T>
struct Monad<std::optional<T>> {
    Monad() = delete;

    using value_type = T;

    template<typename F>
    constexpr static auto
    bind(std::optional<T> && input, F && next) -> std::invoke_result_t<F, T &&>
    {
        if (!input.has_value()) {
            return std::nullopt;
        }

        return next(std::move(*input));
    }
};

#endif // MONAD_IMPL_OPTIONAL_HH_INCLUDED_