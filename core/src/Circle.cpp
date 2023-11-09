#include <Circle.hpp>

#include <iostream>

namespace ink {

Circle::Circle(const Category category, const sf::Color color, float radius,
               sf::Vector2f velocity)
    : Shape(category, color, std::make_unique<sf::CircleShape>(radius)),
      radius_(radius),
      velocity_(std::move(velocity)) {}

void Circle::handleElasticCollision(sf::Vector2f normal) {
  auto angle = velocity_.angleTo(normal);
  // std::cout << "Normal: " << normal.x << ' ' << normal.y << '\n';
  // std::cout << "Angle: " << angle.asDegrees() << '\n';
  velocity_ = velocity_.rotatedBy(2 * angle - sf::degrees(180));
}

void Circle::updateCurrent(sf::Time dt) { move(velocity_ * dt.asSeconds()); }

}  // namespace ink
