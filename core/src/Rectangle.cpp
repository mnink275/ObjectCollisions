#include <Rectangle.hpp>

namespace ink {

Rectangle::Rectangle(const Category category, const sf::Color color,
                     const sf::Vector2f sizes)
    : Shape(category, color, std::make_unique<sf::RectangleShape>(sizes)),
      sizes_(sizes) {
  const float thickness{0.5f};
  const std::vector<sf::Vector2f> bound_sizes{
      {sizes_.x, thickness},    // at top-left
      {-thickness, sizes_.y},   // at top-right
      {-sizes_.x, -thickness},  // at bottom-right
      {thickness, -sizes_.y}    // at bottom-left
  };
  sf::Vector2f normal{0.0f, 1.0f};
  for (std::size_t i = 0; i < shape_->getPointCount(); ++i) {
    sf::FloatRect rect{shape_->getPoint(i), bound_sizes[i]};
    Bound bound{normal, thickness, rect};
    bounds_.push_back(std::move(bound));
    normal = normal.rotatedBy(sf::degrees(90.0f));
  }
}

void Rectangle::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  if (getCategory() == Category::kPlayer) {
    auto x = event.x - sizes_.x / 2;
    auto y = event.y - sizes_.y / 2;
    setShapePosition({x, y});
  }
}

sf::Vector2f Rectangle::getIntersectionNormal(
    sf::FloatRect other) const noexcept {
  auto max_intersection_square = 0.0f;
  sf::Vector2f normal{};
  for (auto&& bound : bounds_) {
    auto intersection = bound.rect.findIntersection(other);
    if (!intersection.has_value()) continue;
    assert(intersection->width >= 0);
    assert(intersection->height >= 0);
    auto square = intersection->width * intersection->height;
    if (square > max_intersection_square) {
      max_intersection_square = square;
      normal = bound.normal;
    }
  }
  assert(normal != sf::Vector2f{});
  return normal;
}

void Rectangle::setShapePosition(sf::Vector2f posision) {
  setPosition(posision);
  if (getCategory() == Category::kObstacle) {
    for (auto&& bound : bounds_) {
      bound.rect.left += posision.x;
      bound.rect.top += posision.y;
    }
  }
}

#if 0
void Rectangle::drawCurrent(sf::RenderTarget& target,
                            const sf::RenderStates states) const {
  target.draw(*shape_, states);

  for (auto&& bound : bounds_) {
    sf::RectangleShape bounds_rect{bound.rect.getSize()};
    bounds_rect.setPosition(bound.rect.getPosition());
    bounds_rect.setFillColor(sf::Color::Blue);
    target.draw(bounds_rect);
  }
}
#endif

}  // namespace ink
