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

struct B {
  static const int id = 2;
  int i = 13;
};

struct C {
  static const int id = 2;
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
      std::cout << a_component->i << "\n";
    }
    m_promise.set_value();
  }

  auto get_future() { return m_promise.get_future(); }

 private:
  ArchetypeArray<Components...>& m_arch;
  std::promise<void> m_promise;
};

TEST(ecstests, threadpool) {
  EntityArray<8> entities;
  ArchetypeArray<A> archetypes;

  auto e = entities.create_entity();
  entities.add_components(e, archetypes.id());
  archetypes.add_entity(e);

  Task_SumInA<A> task(archetypes);
  auto fut = task.get_future();
  tp.schedule_task(&task);
  fut.get();
}
