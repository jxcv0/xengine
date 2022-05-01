#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <mutex>
#include <condition_variable>
#include <limits>

namespace xen::mem
{
    template<typename T>
    struct Allocator
    {
        // required by named requirements
        typedef T value_type;

        // construct a stack allocator with a given size
        Allocator(size_t n)
        {
            if (n > std::numeric_limits<size_t>::max() / sizeof(T))
            {
                throw std::bad_array_new_length();
            }

            _size = n * sizeof(T);
            _top = std::malloc(_size);
            std::cout << _size << " bytes\n";
        }

        // free memory
        ~Allocator()
        {
            std::free(_top);
        }

        // allocate memory from chunk
        T* allocate(size_t nBytes)
        {
            std::unique_lock lk(_m);
            _cv.wait(lk);

            // allocate memory

            lk.unlock();
            _cv.notify_one();
        }

    private: 
        void* _top;
        uintptr_t _bot;
        size_t _size;
        std::mutex _m;
        std::condition_variable _cv;
    };
}// namespace xen::mem

#endif // ALLOC_H
