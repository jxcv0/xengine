#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <mutex>
#include <vector>

#define DEBUG

// TODO stop trying to use this pool allocater as a fix all
namespace xen::mem
{
    static std::mutex mpMutex;
    template<typename T>
    struct MemoryPool
    {
        // singly linked list of free allocations
        struct Node
        {
            Node* next;
        };

        // get the singleton MemoryPool instance static auto& get_instance()
        static auto& get_instance()
        {
            static MemoryPool<T> mp;
            if (!mp._alloc)
            {
                mp.prealloc();
            }
            return mp;
        }

        // get next available allocation and shift _alloc
        T* alloc(size_t n)
        {
            std::lock_guard lk(mpMutex);
            if (!_alloc->next)
            {
                this->prealloc();
            }

            auto a = _alloc;
            _alloc = _alloc->next;
            return reinterpret_cast<T*>(a);
        }

        // return an allocation to the memory pool
        void free(T* ptr, size_t size)
        {
            for (size_t i = 0; i < size; i++)
            {
                std::lock_guard lk(mpMutex);
                // instert ptr back into list
                reinterpret_cast<Node*>(ptr)->next = _alloc;
                _alloc = reinterpret_cast<Node*>(ptr);
            }
        }

    private:

        // malloc more memory for the pool
        void prealloc()
        {
            size_t allocSize = _size * sizeof(T);

            Node *alloc = static_cast<Node*>(std::malloc(allocSize));

            Node *n = alloc;
            for(size_t i = 0; i < _size; i++)
            {
                n->next = reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(n) + sizeof(T));
                n = n->next;
            }

            n->next = nullptr;
            _alloc = alloc;
            _size *= 2;
        }

        MemoryPool() {}
        Node *_alloc = nullptr;
        size_t _size = 1;
    };

    // allocator with pre-allocated memory chunk
    template<typename T>
    struct Allocator
    {
        typedef T value_type;

        Allocator() = default;

        // allocate memory from static resource
        T* allocate(size_t n)
        {
            return _mp.alloc(n);
        }

        // deallocate memory and shift addresses
        void deallocate(T* p, size_t n) noexcept
        {
            _mp.free(p, n);
        }

    private:
        MemoryPool<T> _mp = MemoryPool<T>::get_instance();
    };
}// namespace xen::mem

#endif // ALLOC_H
