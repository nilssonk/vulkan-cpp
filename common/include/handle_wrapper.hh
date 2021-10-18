#ifndef HANDLE_WRAPPER_HH_INCLUDED_
#define HANDLE_WRAPPER_HH_INCLUDED_

#include <utility>

template<typename HandleT, typename Base>
class HandleWrapper {
    HandleT handle_{};

protected:
    [[nodiscard]] constexpr auto
    getHandle() const -> HandleT
    {
        return handle_;
    }

    constexpr auto
    setHandle(HandleT handle) -> void
    {
        handle_ = handle;
    }

public:
    constexpr HandleWrapper() = default;

    HandleWrapper(const HandleWrapper &) = delete;

    constexpr HandleWrapper(HandleWrapper && other) noexcept
        : handle_{std::exchange(other.handle_, HandleT{})}
    {
    }

    virtual ~HandleWrapper()
    {
        Base::closeHandle(handle_);
    }

    auto
    operator=(const HandleWrapper &) -> HandleWrapper & = delete;

    constexpr auto
    operator=(HandleWrapper && other) noexcept -> HandleWrapper &
    {
        handle_ = std::exchange(other.handle_, HandleT{});

        return *this;
    }
};

#endif // HANDLE_WRAPPER_HH_INCLUDED_
