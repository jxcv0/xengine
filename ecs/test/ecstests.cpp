#include <gtest/gtest.h>

#include <future>

#include "archetypearray.h"
#include "entityarray.h"
#include "gtest/gtest.h"
#include "threadpool.h"

ThreadPool tp(4);

struct A {
  static const int id = 1;
  int i = 0;
};

struct B {
  static const int id = 2;
  int i = 13;
};

struct C {
  static const int id = 3;
  int i = 120;
};

template <typename... Components>
class Task_SumInA : public Task {
 public:
  Task_SumInA(ArchetypeArray<Components...>& a) : m_arch(a) {}

  virtual ~Task_SumInA() {}

  int id() override { return (... | Components::id); }

  void process() override {
    for (auto& archetype : m_arch) {
      auto a_component = archetype.template get_component<A>();
      auto b_component = archetype.template get_component<B>();
      auto c_component = archetype.template get_component<C>();
      a_component->i = b_component->i + c_component->i;
    }
    m_promise.set_value();
  }

  auto get_future() { return m_promise.get_future(); }

 private:
  ArchetypeArray<Components...>& m_arch;
  std::promise<void> m_promise;
};

TEST(ecstests, add) {
  A a;
  a.i = 42;
  EntityArray<8> entities;
  ArchetypeArray<A> archetypes;

  auto e = entities.create_entity();
  entities.add_components(e, archetypes.id());
  archetypes.add_entity(e);
  archetypes.set_component(e, a);

  ASSERT_EQ(archetypes.get_component<A>(e)->i, 42);
  ASSERT_TRUE(entities.has_component(e, 1));
}

TEST(ecstests, threadpool) {
  EntityArray<8> entities;
  ArchetypeArray<A, B, C> archetypes;

  auto e = entities.create_entity();
  entities.add_components(e, archetypes.id());
  archetypes.add_entity(e);

  Task_SumInA<A, B, C> task(archetypes);
  auto fut = task.get_future();
  tp.schedule_task(&task);
  fut.get();
  ASSERT_EQ(archetypes.get_component<A>(e)->i, 133);
}
