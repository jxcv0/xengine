#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <mutex>
#include <condition_variable>

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
            std::unique_lock lk(_m);
            cv.wait(_cv);

            size_t size = n * sizeof(T);
            if (auto ptr = static_cast<T*>(_mkr))
            {
                auto marker = reinterpret_cast<uintptr_t>(_mkr);
                marker += size;
                _mkr = reinterpret_cast<void*>(marker);
                return ptr;
            }
            
            lk.unlock();
            cv.notify_one();
        }

        void deallocate(T* ptr, size_t n)
        {
            // TODO
        }

    private:
        static void* _head;
        static void* _mkr;
        std::mutex _m;
        std::condition_variable _cv;
    };
    template<typename T>
    void* Allocator<T>::_head = std::malloc(1048576);    // 1GB
    template<typename T>
    uintptr_t Allocator<T>::_mkr = _head;
}// namespace xen::mem

#endif // ALLOC_H
