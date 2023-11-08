#include <World.hpp>

#include <iostream>
#include <set>

#include <Circle.hpp>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}),
      world_center_(world_bounds_.width / 2, world_bounds_.height / 2),
      player_(nullptr) {
  // spawn player
  auto player = std::make_unique<Rectangle>(Category::kPlayer, sf::Color::Blue,
                                            sf::Vector2f{50.0f, 50.0f});
  player->setShapePosition({0.0f, 0.0f});
  player_ = player.get();
  scene_graph_.attachChild(std::move(player));

  // spawn other objects
  spawnObstacles();
  spawnBalls();
}

void World::update(const sf::Time dt) {
  scene_graph_.update(dt);
  handleCollisions();
}

void World::draw() const {
  window_.setView(world_view_);
  window_.draw(scene_graph_);
}

void World::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  player_->handlePlayerInput(event);
}

void World::handleCollisions() {
  std::set<SceneNode::NodePair> collisions;
  scene_graph_.checkSceneCollision(scene_graph_, collisions);
  for (auto pair : collisions) {
    if (matchesCategories(pair, Category::kPlayer, Category::kObstacle)) {
      std::cout << "Player and Obstacle collision\n";

      assert(dynamic_cast<Rectangle*>(pair.first));
      assert(dynamic_cast<Rectangle*>(pair.second));
      auto* player = static_cast<Rectangle*>(pair.first);
      auto* obstacle = static_cast<Rectangle*>(pair.second);

      player->handleObstacleCollision(obstacle);
    } else if (matchesCategories(pair, Category::kBall, Category::kObstacle)) {
      std::cout << "Ball and Obstacle collision\n";

      assert(dynamic_cast<Circle*>(pair.first));
      assert(dynamic_cast<Rectangle*>(pair.second));
      auto* circle = static_cast<Circle*>(pair.first);
      auto* obstacle = static_cast<Rectangle*>(pair.second);

      auto normal = obstacle->getIntersectionNormal(circle->getBoundingRect());
      circle->handleObstacleCollision(obstacle);
      circle->handleElasticCollision(normal);
    }
  }
}

bool World::matchesCategories(SceneNode::NodePair& pair, Category first,
                              Category second) const noexcept {
  auto first_id = static_cast<CategoryUnderlying>(first);
  auto second_id = static_cast<CategoryUnderlying>(second);
  auto category_id1 =
      static_cast<CategoryUnderlying>(pair.first->getCategory());
  auto category_id2 =
      static_cast<CategoryUnderlying>(pair.second->getCategory());
  if (first_id & category_id1 && second_id & category_id2) {
    return true;
  } else if (first_id & category_id2 && second_id & category_id1) {
    std::swap(pair.first, pair.second);
    return true;
  } else {
    return false;
  }
}

void World::spawnObstacles() {
  sf::Vector2f shift{world_bounds_.width * 0.2f, world_bounds_.height * 0.2f};
  for (auto x = shift.x; x < world_bounds_.width; x += shift.x) {
    for (auto y = shift.y; y < world_bounds_.height; y += shift.y) {
      auto obstacle = std::make_unique<Rectangle>(
          Category::kObstacle, sf::Color::Red, sf::Vector2f{50.0f, 50.0f});
      obstacle->setShapePosition({x, y});
      scene_graph_.attachChild(std::move(obstacle));
    }
  }
}

void World::spawnBalls() {
  auto ball = std::make_unique<Circle>(Category::kBall, sf::Color::Green, 8.0f);
  ball->setShapePosition({20.0f, 20.0f});
  scene_graph_.attachChild(std::move(ball));
}

}  // namespace ink
