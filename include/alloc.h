#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <vector>

// TODO
namespace xen::mem
{
    // allocator with pre-allocated memory chunk
    template<typename T>
    struct Allocator
    {
        // reqd by named requirements
        typedef T value_type;

        Allocator() = default;

        // allocaate memory from static resource
        T* allocate(size_t n)
        {
            std::lock_guard lk(_m);

            size_t size = n * sizeof(T);
            if (auto ptr = static_cast<T*>(_tail))
            {
                auto marker = reinterpret_cast<uintptr_t>(_tail);
                marker += size;
                _mkrs.push_back(marker);
                _tail = reinterpret_cast<void*>(marker);
                return ptr;
            }

            throw std::bad_alloc();
        }

        void deallocate(T* ptr, size_t n) noexcept
        {
            // TODO
        }

    private:
        static void* _head;
        static void* _tail;
        static std::vector<uintptr_t> _mkrs;
        std::mutex _m;
    };

    template<typename T>
    void* Allocator<T>::_head = std::malloc(1048576);    // 1GB
    template<typename T>
    void* Allocator<T>::_tail = _head;
    template<typename T>
    std::vector<uintptr_t> Allocator<T>::_mkrs;
}// namespace xen::mem

#endif // ALLOC_H
