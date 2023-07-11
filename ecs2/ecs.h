#ifndef ECS_H_
#define ECS_H_

#include "entity_mgr.h"
#include "component_array.h"
#include <memory>

namespace xen
{

class ecs
{
public:
    ecs () = default;

    std::uint64_t create_entity ()
    {
        return m_mgr.create_entity();
    }

    void delete_entity (std::uint64_t entity)
    {
        m_mgr.delete_entity(entity);
        std::for_each(m_arrays.begin(), m_arrays.end(), [entity](auto *a) { a->remove (entity); });
    }

    template <typename T>
    void create_component()
    {
        m_arrays.push_back(new component_array<T>);
    }

    template <typename T>
    void add_component (std::uint64_t entity)
    {
        m_mgr.add_component<T> (entity);
        auto arr = find_component_array<T>();
        arr->assign(entity);
    }

    template <typename T>
    void remove_component(std::uint64_t entity)
    {
        auto arr = find_component_array<T>();
        arr->remove(entity);
    }

    template <typename T>
    bool has_component(std::uint64_t entity)
    {
        find_component_array<T>();
        return m_mgr.has_component<T>(entity);
    }

private:
    template <typename T>
    component_array_base *
    find_component_array ()
    {
        auto it = std::find_if (m_arrays.begin(), m_arrays.end(), [] (const auto *ca) { return ca->component_id () == typeid(T).hash_code(); });
        if (it == m_arrays.end())
        {
            throw std::runtime_error("Component type does not exist");
        }
        return *it;
    }

private:
    entity_mgr m_mgr;
    std::vector<component_array_base *> m_arrays;
};

} /* end of namespace xen */

#endif  /* ECS_H_ */ 
