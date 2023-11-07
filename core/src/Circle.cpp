#include <Circle.hpp>

namespace ink {

Circle::Circle(const Category category, const sf::Color color, float radius)
    : Shape(category, color, std::make_unique<sf::CircleShape>(radius)),
      radius_(radius) {}

void Circle::updateCurrent(sf::Time dt) {}

}  // namespace ink
