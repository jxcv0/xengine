#ifndef COMPONENTS_H_
#define COMPONENTS_H_

using ComponentId = int;

namespace components {

constexpr ComponentId mesh = (1 << 0);
constexpr ComponentId translation = (1 << 1);
constexpr ComponentId rotation = (1 << 2);

}  // namespace components

#endif  // COMPONENTS_H_
