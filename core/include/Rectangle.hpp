#pragma once

#include <Shape.hpp>

namespace ink {

class Rectangle : public Shape {
 public:
  using Shape::Shape;
  Rectangle(const Category category, const sf::Color color,
            const sf::Vector2f sizes);

  void handlePlayerInput(const sf::Event::MouseMoveEvent event);

 private:
  const sf::Vector2f sizes_;
};

}  // namespace ink
