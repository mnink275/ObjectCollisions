#include <Shape.hpp>

namespace ink {

Shape::Shape(const Category category, const sf::Color color)
    : SceneNode(category) {
  rectangle_.setSize(rectangle_size);
  rectangle_.setFillColor(color);
}

sf::FloatRect Shape::getBoundingRect() const {
  return getWorldTransform().transformRect(rectangle_.getGlobalBounds());
}

void Shape::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  if (getCategory() == Category::kPlayer) {
    auto x = event.x - rectangle_size.x / 2;
    auto y = event.y - rectangle_size.y / 2;
    setPosition({x, y});
  }
}

void Shape::drawCurrent(sf::RenderTarget& target,
                        const sf::RenderStates states) const {
  target.draw(rectangle_, states);
}

}  // namespace ink
