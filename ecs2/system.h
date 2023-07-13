#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "archetype.h"
#include <vector>

namespace xen
{

template <typename... T>
class system_base
{
public:
  virtual void execute() = 0;

private:
  std::vector<archetype<T...> *> archetypes
};

} /* end of namespace xen */
#endif  /* SYSTEM_H_ */
