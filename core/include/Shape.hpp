#pragma once

#include <SceneNode.hpp>

namespace ink {

class Shape : public SceneNode {
 public:
  using SceneNode::SceneNode;
  Shape(const Category category, const sf::Color color,
        std::unique_ptr<sf::Shape> shape);

  sf::FloatRect getBoundingRect() const override;

 private:
  void drawCurrent(sf::RenderTarget& target,
                   const sf::RenderStates states) const override;

 private:
  std::unique_ptr<sf::Shape> shape_;
};

}  // namespace ink
