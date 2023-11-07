#include <World.hpp>

#include <iostream>
#include <set>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_view_(window.getDefaultView()),
      world_bounds_({0.f, 0.f},
                    {world_view_.getSize().x, world_view_.getSize().y}),
      world_center_(world_bounds_.width / 2, world_bounds_.height / 2),
      player_(nullptr) {
  // init player
  auto player = std::make_unique<Shape>(Category::kPlayer, sf::Color::Blue);
  player->setPosition({0.0f, 0.0f});
  player_ = player.get();
  scene_graph_.attachChild(std::move(player));

  // init obstacles
  auto obstacle = std::make_unique<Shape>(Category::kObstacle, sf::Color::Red);
  obstacle->setPosition(world_center_);
  scene_graph_.attachChild(std::move(obstacle));
}

void World::update(const sf::Time dt) {
  scene_graph_.update(dt);
  handleCollisions();
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
      std::cout << "Player and Obstacle collision!\n";

      assert(dynamic_cast<Shape*>(pair.first));
      auto* player = static_cast<Shape*>(pair.first);
      assert(dynamic_cast<Shape*>(pair.second));
      auto* obstacle = static_cast<Shape*>(pair.second);

      auto intersection_opt = player->getBoundingRect().findIntersection(
          obstacle->getBoundingRect());
      assert(intersection_opt.has_value());
      auto intersection = intersection_opt.value();

      auto is_vertical_collision = intersection.width < intersection.height;
      auto shift = std::min(intersection.width, intersection.height);
      auto player_pos = player->getPosition();
      auto obstacle_pos = obstacle->getPosition();
      if (is_vertical_collision) {
        auto sign = (player_pos.x < obstacle_pos.x) ? -1 : +1;
        std::cout << "Vertical: ";
        std::cout << player_pos.x << ' ' << obstacle_pos.x << '\n';
        player_pos.x += shift * sign;
      } else {
        auto sign = (player_pos.y < obstacle_pos.y) ? -1 : +1;
        std::cout << "Horizontal: ";
        std::cout << player_pos.y << ' ' << obstacle_pos.y << '\n';
        player_pos.y += shift * sign;
      }
      player->setPosition(player_pos);
    }
  }
}

}  // namespace ink
