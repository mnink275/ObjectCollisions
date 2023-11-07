#pragma once

#include <Shape.hpp>

namespace ink {

class Circle : public Shape {
 public:
  using Shape::Shape;
  Circle(const Category category, const sf::Color color, float radius);

 private:
  void updateCurrent(sf::Time dt) override;

 private:
  const float radius_;
};

}  // namespace ink
