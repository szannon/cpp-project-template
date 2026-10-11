#pragma once
#include <a_library/linealg/VectorBase.hpp>

namespace a_library::linealg {
template <typename T>
using Vec2 = VectorBase<T, 2>;

template <typename T>
using Vec3 = VectorBase<T, 3>;
}  // namespace a_library::linealg
