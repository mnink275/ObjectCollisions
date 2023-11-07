#include <Player.hpp>

namespace ink {

Shape::Shape(Category category, sf::Color color, sf::Vector2f init_pos) : SceneNode(category) {
  rectangle_.setSize(rectangle_size);
  rectangle_.setFillColor(color);
  rectangle_.setPosition(init_pos);
}

sf::FloatRect Shape::getBoundingRect() const {
  return rectangle_.getGlobalBounds();
}

void Shape::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  if (getCategory() == Category::kPlayer) {
    auto x = event.x - rectangle_size.x / 2;
    auto y = event.y - rectangle_size.y / 2;
    rectangle_.setPosition({x, y});
  }
}

void Shape::drawCurrent(sf::RenderTarget& target,
                           const sf::RenderStates states) const {
  target.draw(rectangle_, states);
}

void Shape::updateCurrent(sf::Time) {}

}  // namespace ink
