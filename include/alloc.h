#ifndef ALLOC_H
#define ALLOC_H

#include "checkerr.h"
#include <cstdlib>

// #define MEM_DEBUG

#ifdef MEM_DEBUG
#include <string>
#endif

// TODO general heap allocator
// TODO block (SLAB/SLOB?) allocator 

namespace xen::mem
{
    template<typename T>
    struct StackAllocator
    {
        StackAllocator(size_t n)
        {
            auto bytes = (n * sizeof(T)) + alignof(T);
            _start = reinterpret_cast<uintptr_t>(malloc(bytes));
            
            // align
            size_t mask = alignof(T) - 1;
            assert((alignof(T) & mask) == 0); // pwr of 2
            _start = (_start + mask) & ~mask;
            _mkr = _start;
            _end = _start + bytes;
        }

        ~StackAllocator()
        {
            clear();
            free(reinterpret_cast<void*>(_start));
        }
        
        T* allocate(size_t n)
        {
            auto bytes = n * sizeof(T);
            auto temp = reinterpret_cast<void*>(_mkr);
            _mkr += bytes;

#ifdef MEM_DEBUG
            std::string msg("alloc: " + std::to_string(_mkr));
            logmsg(msg.c_str());
#endif

            return static_cast<T*>(temp);
        }

        void deallocate(T* ptr)
        {
            _mkr = reinterpret_cast<uintptr_t>(ptr);

#ifdef MEM_DEBUG
            std::string msg("free:  " + std::to_string(_mkr));
            logmsg(msg.c_str());
#endif

        }

        void clear()
        {
            _mkr = _start;

#ifdef MEM_DEBUG
            std::string msg("clear: " + std::to_string(_mkr));
            logmsg(msg.c_str());
#endif
        }

    private:
        uintptr_t _start;
        uintptr_t _end;
        uintptr_t _mkr;
    };
}// namespace xen::mem

#endif // ALLOC_H
