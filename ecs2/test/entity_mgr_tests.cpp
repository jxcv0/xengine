#include <gtest/gtest.h>
#include "entity_mgr.h"

TEST (entity_mgr_tests, create_and_delete_entities)
{
    xen::entity_mgr mgr;
    std::uint64_t e1 = mgr.create_entity ();
    ASSERT_EQ (e1, 0);
    ASSERT_EQ (mgr.num_entities(), 1);

    mgr.delete_entity (e1);
    ASSERT_EQ (mgr.num_entities (), 0);
    std::uint64_t e2 = mgr.create_entity ();
    ASSERT_EQ (mgr.num_entities (), 1);
    ASSERT_EQ (e1, e2); /* ID should be reused */
}

TEST (entity_mgr_tests, add_remove_components)
{
    struct vec3
    {
        float m_elem[3];
    };

    xen::entity_mgr mgr;
    std::uint64_t e1 = mgr.create_entity ();
    ASSERT_FALSE (mgr.has_component<vec3>(e1));
    mgr.add_component<vec3>(e1);
    ASSERT_TRUE (mgr.has_component<vec3>(e1));
}
