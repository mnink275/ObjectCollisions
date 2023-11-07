#pragma once

#include <cassert>
#include <memory>
#include <set>

#include <SFML/Graphics.hpp>

#include <Category.hpp>

namespace ink {

class SceneNode : public sf::Transformable, public sf::Drawable {
 public:
  using Ptr = std::unique_ptr<SceneNode>;
  using NodePair = std::pair<SceneNode*, SceneNode*>;

  SceneNode(Category category = Category::kNone);
  virtual ~SceneNode() = default;

  SceneNode(const SceneNode&) = delete;
  SceneNode& operator=(const SceneNode&) = delete;

  SceneNode(SceneNode&&) noexcept = default;
  SceneNode& operator=(SceneNode&&) noexcept = default;

  void attachChild(Ptr child);
  Ptr detachChild(const SceneNode& node);
  void update(const sf::Time dt);

  void checkSceneCollision(SceneNode& scene_root,
                           std::set<NodePair>& collisions);
  void checkNodeCollision(SceneNode& node, std::set<NodePair>& collisions);
  virtual sf::FloatRect getBoundingRect() const;

  sf::Transform getWorldTransform() const;
  Category getCategory() const noexcept;

 private:
  void draw(sf::RenderTarget& target,
            const sf::RenderStates& states) const override final;
  virtual void drawCurrent(sf::RenderTarget& target,
                           const sf::RenderStates states) const;
  virtual void updateCurrent(sf::Time dt);
  void updateChildren(sf::Time dt) const;
  sf::Vector2f getWorldPosition() const;
  bool isCollide(const SceneNode& lhs, const SceneNode& rhs) const;

  std::vector<Ptr> children_;
  SceneNode* parent_;
  Category category_;
};

}  // namespace ink
