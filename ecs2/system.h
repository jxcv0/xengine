#ifndef SYSTEM_H_
#define SYSTEM_H_

#include <execution>
#include <memory>
#include <vector>

#include "archetype.h"

namespace xen
{

template <typename Func, typename... T> class system
{
public:
  system(Func f) : m_func{ f } {}

  void
  iterate()
  {
    std::for_each(std::execution::par_unseq, m_archetypes.begin(),
                  m_archetypes.end(), m_func);
  }

  void
  add_archetype()
  {
  }

private:
  std::vector<std::shared_ptr<archetype_base> > m_archetypes;
  Func m_func;
};

} /* end of namespace xen */

#endif /* SYSTEM_H_ */
