#pragma once

#include <Shape.hpp>

namespace ink {

class Circle final : public Shape {
 public:
  using Shape::Shape;
  Circle(const Category category, const sf::Color color, float radius);

  void handleElasticCollision(sf::Vector2f normal);

 private:
  void updateCurrent(sf::Time dt) override;

 private:
  const float radius_;
  sf::Vector2f velocity_;
};

}  // namespace ink
