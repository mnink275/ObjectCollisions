#pragma once

#include <SceneNode.hpp>

namespace ink {

class Shape : public SceneNode {
 public:
 public:
  using SceneNode::SceneNode;
  Shape(Category category, sf::Color color, sf::Vector2f init_pos);

  sf::FloatRect getBoundingRect() const override;
  void handlePlayerInput(const sf::Event::MouseMoveEvent event);

 private:
  void drawCurrent(sf::RenderTarget& target,
                           const sf::RenderStates states) const override;
  void updateCurrent(sf::Time) override;

 private:
  const sf::Vector2f rectangle_size{50.0f, 50.0f};
  sf::RectangleShape rectangle_;
};

}  // namespace ink
