#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <vector>

#define DEBUG

namespace xen::mem
{
    struct Memory
    {
        void* _head = std::malloc(1048576);
        void* _tail = _head;
        std::vector<uintptr_t> _mkrs;
        std::mutex _m;
    };
} // namespace xen::mem

extern xen::mem::Memory MEMORY_RESOURCE;

namespace xen::mem
{
    // allocator with pre-allocated memory chunk
    template<typename T>
    struct Allocator
    {
        // reqd by named requirements
        typedef T value_type;

        Allocator() = default;

        // allocate memory from static resource
        T* allocate(size_t n)
        {
            std::lock_guard lk(MEMORY_RESOURCE._m);

            size_t size = n * sizeof(T);
            if (auto ptr = static_cast<T*>(MEMORY_RESOURCE._tail))
            {
                auto marker = reinterpret_cast<uintptr_t>(MEMORY_RESOURCE._tail);
                marker += size;
                MEMORY_RESOURCE._mkrs.push_back(marker);
                MEMORY_RESOURCE._tail = reinterpret_cast<void*>(marker);
#ifdef DEBUG
                std::cout << "Allocating " << size << " bytes -> " << ptr << "\n";
#endif
                return ptr;
            }

            throw std::bad_alloc();
        }

        // deallocate memory and shift addresses
        void deallocate(T* ptr, size_t n) noexcept
        {
            
        }
    };
}// namespace xen::mem

#endif // ALLOC_H
