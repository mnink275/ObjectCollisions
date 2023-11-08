#pragma once

#include <SceneNode.hpp>

namespace ink {

class Shape : public SceneNode {
 public:
  using SceneNode::SceneNode;
  Shape(const Category category, const sf::Color color,
        std::unique_ptr<sf::Shape> shape);

  sf::FloatRect getBoundingRect() const override;
  virtual void setShapePosition(sf::Vector2f posision);
  void handleObstacleCollision(Shape* obstacle);

 private:
  void drawCurrent(sf::RenderTarget& target,
                   const sf::RenderStates states) const override;

 protected:
  std::unique_ptr<sf::Shape> shape_;
};

}  // namespace ink
