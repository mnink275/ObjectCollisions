#include <World.hpp>

#include <random>
#include <set>

#include <Circle.hpp>

namespace ink {

World::World(sf::RenderWindow& window)
    : window_(window),
      world_bounds_({0.f, 0.f}, sf::Vector2f{window.getSize()}),
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

  // spawn world bounds
  spawnWalls();
}

void World::update(const sf::Time dt) {
  scene_graph_.update(dt);
  handleCollisions();
}

void World::draw() const { window_.draw(scene_graph_); }

void World::handlePlayerInput(const sf::Event::MouseMoveEvent event) {
  player_->handlePlayerInput(event);
}

void World::handleCollisions() {
  std::set<SceneNode::NodePair> collisions;
  scene_graph_.checkSceneCollision(scene_graph_, collisions);
  for (auto pair : collisions) {
    if (matchesCategories(pair, Category::kPlayer, Category::kObstacle)) {
      // std::cout << "Player and Obstacle collision\n";
      assert(dynamic_cast<Rectangle*>(pair.first));
      assert(dynamic_cast<Rectangle*>(pair.second));
      auto* player = static_cast<Rectangle*>(pair.first);
      auto* obstacle = static_cast<Rectangle*>(pair.second);

      player->handleObstacleCollision(obstacle);
    } else if (matchesCategories(pair, Category::kBall, Category::kObstacle) ||
               matchesCategories(pair, Category::kBall, Category::kPlayer)) {
      // std::cout << "Ball and Obstacle collision\n";
      assert(dynamic_cast<Circle*>(pair.first));
      assert(dynamic_cast<Rectangle*>(pair.second));
      auto* circle = static_cast<Circle*>(pair.first);
      auto* obstacle = static_cast<Rectangle*>(pair.second);

      auto normal = obstacle->getIntersectionNormal(circle->getBoundingRect());
      if (normal.has_value()) {
        circle->handleObstacleCollision(obstacle);
        circle->handleElasticCollision(normal.value());
      }
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

float World::getRandomBetween(int left, int right) {
  static std::random_device seed;
  static std::mt19937 gen(seed());
  std::uniform_int_distribution<> distrib(left, right);
  return distrib(gen);
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
  const int velocity_bound = 500;
  auto rand = []() {
    return getRandomBetween(-velocity_bound, velocity_bound);
  };
  const auto spawn_begin = 10 * world_bounds_thickness_;
  const auto spawn_end = world_bounds_.width - spawn_begin;
  const auto spawn_height = 2 * world_bounds_thickness_;
  const auto spawn_interval = 20.0f;
  for (float x = spawn_begin; x < spawn_end; x += spawn_interval) {
    sf::Vector2f velocity{rand(), rand()};
    auto ball = std::make_unique<Circle>(Category::kBall, sf::Color::Green,
                                         8.0f, velocity);
    ball->setShapePosition({x, spawn_height});
    scene_graph_.attachChild(std::move(ball));
  }
}

void World::spawnWalls() {
  auto walls_holder = std::make_unique<SceneNode>();

  const auto world_bounds = world_bounds_.getSize();
  const float width = world_bounds.x;
  const float height = world_bounds.y;

  // `positions` contains left-top corner of the walls
  // walls "grow" from the left-top corner to the bottom-right
  static const std::vector<sf::Vector2f> positions = {
      {0.0f, 0.0f},                               // left
      {0.0f, 0.0f},                               // top
      {width - world_bounds_thickness_, 0.0f},    // right
      {0.0f, height - world_bounds_thickness_}};  // bottom
  static const std::vector<sf::Vector2f> sizes = {
      {world_bounds_thickness_, height},  // left
      {width, world_bounds_thickness_},   // top
      {world_bounds_thickness_, height},  // right
      {width, world_bounds_thickness_}};  // bottom

  assert(positions.size() == sizes.size());
  static const std::size_t kWallsCount = positions.size();

  for (std::size_t i = 0; i < kWallsCount; ++i) {
    auto wall = std::make_unique<Rectangle>(Category::kObstacle, sf::Color::Red,
                                            sizes[i]);
    wall->setShapePosition(positions[i]);
    walls_holder->attachChild(std::move(wall));
  }

  scene_graph_.attachChild(std::move(walls_holder));
}

}  // namespace ink
