#ifndef PHANTOMPI_HANDLE_HPP
#define PHANTOMPI_HANDLE_HPP 1

#include <utility>

namespace phantompi
{
    template <typename T, typename CLOSE_FN>
    class Handle
    {
    public:
        Handle(T t, CLOSE_FN closeFn);

        T& operator * () noexcept;

        T const& operator * () const noexcept;

        T* operator -> () noexcept;

        T const* operator -> () const noexcept;

        ~Handle() noexcept;

    private:
        T _t;
        CLOSE_FN _closeFn;
    };

    template <typename T, typename CLOSE_FN>
    auto makeHandle(T t, CLOSE_FN closeFn)
    {
        return Handle<T, CLOSE_FN>{std::move(t), std::move(closeFn)};
    }

    template <typename T, typename CLOSE_FN>
    inline Handle<T, CLOSE_FN>::Handle(T t, CLOSE_FN closeFn)
      : _t{std::move(t)},
        _closeFn{std::move(closeFn)} { }

    template <typename T, typename CLOSE_FN>
    inline T& Handle<T, CLOSE_FN>::operator * () noexcept
    {
        return _t;
    }

    template <typename T, typename CLOSE_FN>
    inline T const& Handle<T, CLOSE_FN>::operator * () const noexcept
    {
        return _t;
    }

    template <typename T, typename CLOSE_FN>
    T* Handle<T, CLOSE_FN>::operator -> () noexcept
    {
        return &_t;
    }

    template <typename T, typename CLOSE_FN>
    T const* Handle<T, CLOSE_FN>::operator -> () const noexcept
    {
        return &_t;
    }

    template <typename T, typename CLOSE_FN>
    inline Handle<T, CLOSE_FN>::~Handle() noexcept
    {
        _closeFn();
    }
}

#endif
