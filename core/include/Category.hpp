#pragma once

namespace ink {

using CategoryUnderlying = unsigned int;

enum class Category : CategoryUnderlying {
  kNone = 0,
  kPlayer = 1 << 0,
  kObstacle = 1 << 1,
  kBall = 1 << 2,
};

}  // namespace ink
