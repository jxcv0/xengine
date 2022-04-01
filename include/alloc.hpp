#ifndef _ALLOC_HPP_
#define _ALLOC_HPP_

#include <assert.h>
#include <cstdint>

namespace ge::alloc {

    /**
     * @brief Shift and address upwards as necessary to align to a given number of bytes
     * 
     * @tparam T - any type
     * @param ptr the ptr to align
     * @param align the alignment
     * @return T* the aligned ptr
     */
    template<typename T>
    inline T* align_ptr(T* ptr, std::size_t align) {
        const std::size_t mask = align - 1;
        assert((align & mask) == 0);    // ^2
        const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
        const uintptr_t addr_aligned = (addr + mask) & ~mask;
        return reinterpret_cast<T*>(addr_aligned);
    }

    /**
     * @brief Get a pointer to an aligned memory address
     * 
     * @param bytes the number of bytes to allocate
     * @param align the alignment (MUST BE POWER OF 2)
     * @return void* the ptr to the address
     */
    void* alloc_aligned(std::size_t bytes, std::size_t align) {
        std::size_t actual_bytes = bytes + align;
        uint8_t* raw_ptr = new uint8_t[actual_bytes];
        uint8_t* aligned_ptr = align_ptr(raw_ptr, align);
        
        if (aligned_ptr == raw_ptr) {
            aligned_ptr += align;
        }

        std::ptrdiff_t shift = aligned_ptr - raw_ptr;
        assert(shift > 0 && shift <= 256);
        aligned_ptr[-1] = static_cast<uint8_t>(shift & 0xFF);
        return aligned_ptr;
    }

    /**
     * @brief Free aligned memory
     * 
     * @param ptr ptr to memory
     */
    void free_aligned(void* ptr) {
        if (ptr) {
            uint8_t *aligned_ptr = reinterpret_cast<uint8_t*>(ptr);
            std::ptrdiff_t shift = aligned_ptr[-1];
            if (shift == 0) {
                shift = 256;
            }
            uint8_t *raw_ptr = aligned_ptr - shift;
            delete[] raw_ptr;
        }
    }

    /**
     * @brief A stack allocator that manages a contigious block of memory via a pointer to the top
     * of the memory stack. Addresses below this ptr are considered to be in use and addresses
     * above are considered free. The top pointer is initialized to the lowest memory address in
     * the stack. Each allocation request simply moves the pointer up by the requested number of
     * bytes. The most recently allocated block is freed by moving the top pointer back down by
     * the size of the freed block.
     * 
     */
    class stack {

        /**
         * @brief Pointer to the top of the stack
         * 
         */
        void* top_;

        /**
         * @brief the amount of allocated memory
         * 
         */
        std::size_t size_;

    public:

    };
} // namespace ge::alloc

#endif // _ALLOC_HPP_