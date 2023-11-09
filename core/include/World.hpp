#pragma once

#include <SFML/Graphics.hpp>

#include <Category.hpp>
#include <Rectangle.hpp>
#include <SceneNode.hpp>

namespace ink {

class World final {
  using Ptr = SceneNode::Ptr;

 public:
  explicit World(sf::RenderWindow& window);

  World(const World&) = delete;
  World& operator=(const World&) = delete;

  World(World&&) = delete;
  World& operator=(World&&) = delete;

  void update(sf::Time dt);
  void draw() const;
  void handlePlayerInput(const sf::Event::MouseMoveEvent event);
  void handleCollisions();

 private:
  bool matchesCategories(SceneNode::NodePair& pair, Category first,
                         Category second) const noexcept;
  void spawnObstacles();
  void spawnBalls();
  void spawnWalls();

  SceneNode scene_graph_;
  sf::RenderWindow& window_;
  sf::FloatRect world_bounds_;
  sf::Vector2f world_center_;
  const float walls_thickness_ = 10.0f;

  Rectangle* player_;
};

}  // namespace ink
