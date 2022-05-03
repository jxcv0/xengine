#ifndef ALLOC_H
#define ALLOC_H

#include <cstdlib>
#include <cstring>
#include <atomic>
#include <mutex>
#include <vector>

#define DEBUG

namespace xen::mem
{
    template<typename T>
    struct MemoryPool
    {
        // singly linked list of free allocations
        struct Node
        {
            Node* next;
        };

        // get the singleton MemoryPool instance static auto& get_instance()
        {
            static MemoryPool<T> mp;
            if (!mp._alloc)
            {
                // TODO - use ^2 allocs
                size_t allocSize = 100 * sizeof(T);
                Node *alloc = static_cast<Node*>(std::malloc(allocSize));
                Node *n = alloc;
            
                for(size_t i = 0; i < 100; i++)
                {
                    n->next = reinterpret_cast<Node*>(reinterpret_cast<uintptr_t>(n) + sizeof(T));
                    n = n->next;
                }

                n->next = nullptr;
                mp._alloc = alloc;
            }
            return mp;
        }

        // get next available allocation and shift _alloc
        T* get_alloc()
        {
            auto a = _alloc;
            _alloc = _alloc->next;
            return reinterpret_cast<T*>(a);
        }

        // return an allocation to the memory pool
        void return_alloc(T* ptr, size_t size)
        {
            // instert ptr back into list
            reinterpret_cast<Node*>(ptr)->next = _alloc;
            _alloc = reinterpret_cast<Node*>(ptr);
        }

    private:
        MemoryPool() {}
        Node *_alloc = nullptr;
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
            return _mp.get_alloc();
        }

        // deallocate memory and shift addresses
        void deallocate(T* p, size_t n) noexcept
        {
            _mp.return_alloc(p, n);
        }

    private:
        MemoryPool<T> _mp = MemoryPool<T>::get_instance();
    };
}// namespace xen::mem

#endif // ALLOC_H
