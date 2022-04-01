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
        std::size_t worst_case = bytes + align - 1;
        int8_t* raw_mem = new int8_t[worst_case];
        return align_ptr(raw_mem, align);
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