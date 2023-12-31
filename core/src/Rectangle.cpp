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
    Bound bound{shape_->getPoint(i), normal, thickness, rect};
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

std::optional<sf::Vector2f> Rectangle::getIntersectionNormal(
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

  if (normal == sf::Vector2f{}) return std::nullopt;

  return normal;
}

void Rectangle::setShapePosition(sf::Vector2f posision) {
  setPosition(posision);
  for (auto&& bound : bounds_) {
    bound.rect.left = bound.initial_pos.x + posision.x;
    bound.rect.top = bound.initial_pos.y + posision.y;
  }
}

}  // namespace ink
