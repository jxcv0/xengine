#include <gtest/gtest.h>

#include "archetype.hpp"

TEST(archetype_tests, create_info)
{
  auto info1 = xen::create_component_info<int>();
  xen::component_info int_info{ std::type_index(typeid(int)), sizeof(int) };
  ASSERT_EQ(info1[0], int_info);

  auto info2 = xen::create_component_info<int, float, char>();
  xen::component_info float_info{ std::type_index(typeid(float)),
                                  sizeof(float) };
  xen::component_info char_info{ std::type_index(typeid(char)), sizeof(char) };
  ASSERT_EQ(info2[0], int_info);
  ASSERT_EQ(info2[1], float_info);
  ASSERT_EQ(info2[2], char_info);
}

TEST(archetype_tests, has_component)
{
  struct position
  {
    float xyz[3];
  };

  xen::archetype arch(xen::create_component_info<float, char, position>());
  ASSERT_TRUE(arch.has_component(std::type_index(typeid(char))));
  ASSERT_TRUE(arch.has_component(std::type_index(typeid(position))));
  ASSERT_TRUE(arch.has_component(std::type_index(typeid(float))));
  ASSERT_FALSE(arch.has_component(std::type_index(typeid(int))));
  ASSERT_FALSE(arch.has_component(std::type_index(typeid(double))));
  ASSERT_FALSE(arch.has_component(std::type_index(typeid(long))));

  ASSERT_TRUE(arch.has_component<char>());
  ASSERT_TRUE(arch.has_component<position>());
  ASSERT_TRUE(arch.has_component<float>());
  ASSERT_FALSE(arch.has_component<int>());
  ASSERT_FALSE(arch.has_component<double>());
  ASSERT_FALSE(arch.has_component<long>());
}

TEST(archetype_tests, components_size)
{
  xen::archetype arch(xen::create_component_info<float, char, int>());
  ASSERT_EQ(arch.components_size(), 9);
}

TEST(archetype_tests, num_entities)
{
  xen::archetype arch(xen::create_component_info<float, char, int>());
  ASSERT_EQ(arch.num_entities(), 0);
  arch.add_entity(1);
  ASSERT_EQ(arch.num_entities(), 1);
  arch.add_entity(2);
  ASSERT_EQ(arch.num_entities(), 2);
}

TEST(archetype_tests, get_component)
{
  xen::archetype arch(xen::create_component_info<float, char, int>());
  arch.add_entity(1);
  arch.add_entity(2);
  int* i1
      = static_cast<int*>(arch.get_component(1, std::type_index(typeid(int))));
  *i1 = 10;
  ASSERT_EQ(
      *static_cast<int*>(arch.get_component(1, std::type_index(typeid(int)))),
      10);

  int* i2
      = static_cast<int*>(arch.get_component(2, std::type_index(typeid(int))));
  *i2 = 1001;
  ASSERT_EQ(
      *static_cast<int*>(arch.get_component(2, std::type_index(typeid(int)))),
      1001);
  ASSERT_EQ(
      *static_cast<int*>(arch.get_component(1, std::type_index(typeid(int)))),
      10);
}

TEST(archetype_tests, get_component_template)
{
  xen::archetype arch(xen::create_component_info<float, char, int>());
  arch.add_entity(1);
  arch.add_entity(2);
  float& f1 = arch.get_component<float>(1);
  f1 = 42.0f;
  float& f2 = arch.get_component<float>(2);
  f2 = 123.456f;
  ASSERT_FLOAT_EQ(arch.get_component<float>(1), 42.0f);
  ASSERT_FLOAT_EQ(arch.get_component<float>(2), 123.456f);
}

TEST(archetype_tests, get_component_info)
{
  xen::archetype arch(xen::create_component_info<float, char, int>());
  auto info1 = arch.get_component_info();
  auto info2 = xen::create_component_info<float, char, int>();
  ASSERT_EQ(info1, info2);
}

TEST(archetype_tests, remove_entity)
{
  xen::archetype arch(xen::create_component_info<int>());
  arch.add_entity(1);
  arch.add_entity(2);
  arch.add_entity(3);
  arch.get_component<int>(1) = 1;
  arch.get_component<int>(2) = 2;
  arch.get_component<int>(3) = 3;
  arch.remove_entity(2);
  ASSERT_EQ(arch.get_component<int>(1), 1);
  ASSERT_EQ(arch.get_component<int>(3), 3);
}