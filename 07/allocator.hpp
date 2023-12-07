#pragma once


namespace myalloc {
    template <class T>
    class allocator {
    public:
        using value_type = T;
        using size_type = size_t;

        constexpr allocator() noexcept {}
        constexpr allocator(const allocator&) noexcept = default;
        template <class _Other>
        constexpr allocator(const allocator<_Other>&) noexcept {}
        ~allocator() = default;
        allocator& operator=(const allocator&) = default;

        void deallocate(T* ptr, const size_t count) { free(ptr); }
        T* allocate(const size_t count) { return reinterpret_cast<T*>(malloc(sizeof(T) * count)); }
    };
}