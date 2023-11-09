#pragma once

#include <Shape.hpp>

namespace ink {

class Rectangle final : public Shape {
 public:
  struct Bound final {
    sf::Vector2f initial_pos{};
    sf::Vector2f normal{};
    float thickness{};
    sf::FloatRect rect{};
  };

 public:
  using Shape::Shape;
  Rectangle(const Category category, const sf::Color color,
            const sf::Vector2f sizes);

  void handlePlayerInput(const sf::Event::MouseMoveEvent event);
  std::optional<sf::Vector2f> getIntersectionNormal(
      sf::FloatRect other) const noexcept;
  void setShapePosition(sf::Vector2f posision) override;

 private:
  const sf::Vector2f sizes_;
  std::vector<Bound> bounds_;
};

}  // namespace ink
