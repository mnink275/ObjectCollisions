#include <Rectangle.hpp>

namespace ink {

Rectangle::Rectangle(const Category category, const sf::Color color,
                     const sf::Vector2f sizes)
    : Shape(category, color, std::make_unique<sf::RectangleShape>(sizes)),
      sizes_(sizes) {}

void Rectangle::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  if (getCategory() == Category::kPlayer) {
    auto x = event.x - sizes_.x / 2;
    auto y = event.y - sizes_.y / 2;
    setPosition({x, y});
  }
}

}  // namespace ink
