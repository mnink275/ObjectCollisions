#include <SceneNode.hpp>

#include <algorithm>

namespace ink {

SceneNode::SceneNode(Category category)
    : children_(), parent_(nullptr), category_(category) {}

void SceneNode::attachChild(Ptr child) {
  child->parent_ = this;
  children_.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node) {
  const auto found =
      std::find_if(children_.begin(), children_.end(),
                   [&](const Ptr& p) -> bool { return p.get() == &node; });
  assert(found != children_.end());
  Ptr result = std::move(*found);
  result->parent_ = nullptr;
  children_.erase(found);
  return result;
}

void SceneNode::update(const sf::Time dt) {
  updateCurrent(dt);
  updateChildren(dt);
}

void SceneNode::checkSceneCollision(SceneNode& scene_root,
                                    std::set<SceneNode::NodePair>& collisions) {
  checkNodeCollision(scene_root, collisions);
  for (auto&& child : scene_root.children_) {
    checkSceneCollision(*child, collisions);
  }
}

void SceneNode::checkNodeCollision(SceneNode& node,
                                   std::set<SceneNode::NodePair>& collisions) {
  if (this != &node && isCollide(*this, node)) {
    collisions.insert(std::minmax(this, &node));
  }
  for (auto&& child : children_) {
    child->checkNodeCollision(node, collisions);
  }
}

sf::FloatRect SceneNode::getBoundingRect() const { return sf::FloatRect{}; }

sf::Transform SceneNode::getWorldTransform() const {
  sf::Transform transform = sf::Transform::Identity;
  for (auto node = this; node != nullptr; node = node->parent_) {
    transform *= node->getTransform();
  }

  return transform;
}

Category SceneNode::getCategory() const noexcept { return category_; }

void SceneNode::draw(sf::RenderTarget& target,
                     const sf::RenderStates& states) const {
  sf::RenderStates node_states(states);
  node_states.transform *= getTransform();
  drawCurrent(target, node_states);
  for (const Ptr& child : children_) {
    child->draw(target, node_states);
  }
}

void SceneNode::drawCurrent(sf::RenderTarget& /*target*/,
                            const sf::RenderStates /*states*/) const {}

void SceneNode::updateCurrent(sf::Time) {}

void SceneNode::updateChildren(const sf::Time dt) const {
  for (auto&& child : children_) {
    child->update(dt);
  }
}

sf::Vector2f SceneNode::getWorldPosition() const {
  return getWorldTransform() * sf::Vector2f();
}

bool SceneNode::isCollide(const SceneNode& lhs, const SceneNode& rhs) const {
  return lhs.getBoundingRect()
      .findIntersection(rhs.getBoundingRect())
      .has_value();
}

}  // namespace ink
