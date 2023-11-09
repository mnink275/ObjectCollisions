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

void Shape::setShapePosition(sf::Vector2f posision) { setPosition(posision); }

void Shape::handleObstacleCollision(Shape* obstacle) {
  auto intersection_opt =
      getBoundingRect().findIntersection(obstacle->getBoundingRect());
  if (!intersection_opt.has_value()) return;
  const auto intersection = intersection_opt.value();

  auto is_vertical_collision = intersection.width < intersection.height;
  const auto shift = std::min(intersection.width, intersection.height);
  auto shape_pos = getPosition();
  const auto obstacle_pos = obstacle->getPosition();
  if (is_vertical_collision) {
    auto sign = (shape_pos.x < obstacle_pos.x) ? -1 : +1;
    shape_pos.x += shift * sign;
  } else {
    auto sign = (shape_pos.y < obstacle_pos.y) ? -1 : +1;
    shape_pos.y += shift * sign;
  }
  setShapePosition(shape_pos);
}

void Shape::drawCurrent(sf::RenderTarget& target,
                        const sf::RenderStates states) const {
  target.draw(*shape_, states);
}

}  // namespace ink
