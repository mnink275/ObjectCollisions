#include <Shape.hpp>

namespace ink {

Shape::Shape(const Category category, const sf::Color color,
             std::unique_ptr<sf::Shape> shape)
    : SceneNode(category), shape_(std::move(shape)) {
  shape_->setFillColor(color);
}

sf::FloatRect Shape::getBoundingRect() const {
  return getWorldTransform().transformRect(shape_->getGlobalBounds());
}

void Shape::drawCurrent(sf::RenderTarget& target,
                        const sf::RenderStates states) const {
  target.draw(*shape_, states);
}

}  // namespace ink
