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

inline uintptr_t align_addr(uintptr_t addr, size_t align)
{
    const size_t mask = align - 1;
    assert((align & mask) == 0); // pwr of 2
    return 
}

// Aligned allocation function. IMPORTANT: 'align'
// must be a power of 2 (typically 4, 8 or 16).
template<typename T>
T* alloc_aligned(size_t n)
{
    auto align = alignof(T);
    size_t nBytes = ((n * sizeof(T)) + align;
    uintptr_t rawPtr = reinterpret_cast<uintptr_t>(nBytes);

    auto mask = align - 1;
    assert((align & mask) == 0);
    auto alignedPtr = (rawPtr + mask) & ~mask;

    // shift addr if already aligned
    if (ptr == rawPtr) { alignedPtr += align; }

    // store shift
    ptrdiff_t shift = alignedPtr - rawPtr;
    assert(shift > 0 && shift <= 256);

    alignedPtr[-1] = static_cast<uintptr_t>(shift & 0xFF);
    return alignedPtr;
}

void freealigned(void* pmem)
{
    if (pmem)
    {
        // convert to u8 pointer.
        u8* palignedmem = reinterpret_cast<u8*>(pmem);
        // extract the shift.
        ptrdiff_t shift = palignedmem[-1];
        if (shift == 0)
        shift = 256;
        // back up to the actual allocated address,
        // and array-delete it.
        u8* prawmem = palignedmem - shift;
        delete[] prawmem;
    }
}

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
        
        // allocate memory
        T* allocate(size_t n)
        {
            auto bytes = n * sizeof(T);
            auto temp = reinterpret_cast<void*>(_mkr);
            _mkr += bytes;

            if (_mkr > _end)
            {
                logmsg("ERROR::OUT_OF_MEMORY");
                exit(1);
            }

#ifdef MEM_DEBUG
            std::string msg("alloc: " + std::to_string(_mkr));
            logmsg(msg.c_str());
#endif

            return static_cast<T*>(temp);
        }

        // deallocate up to a ptr
        void deallocate(T* ptr)
        {
            _mkr = reinterpret_cast<uintptr_t>(ptr);

#ifdef MEM_DEBUG
            std::string msg("free:  " + std::to_string(_mkr));
            logmsg(msg.c_str());
#endif
        }

        // clear entire stack
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
} // namespace xen::mem

#endif // ALLOC_H
