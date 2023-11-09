#include <Game.hpp>

namespace ink {

Game::Game()
    : window_(sf::VideoMode({1280, 720}), "ObjectCollisions", sf::Style::Close),
      world_(window_) {
  window_.setPosition({0, 0});
  window_.setView(window_.getDefaultView());
}

void Game::run() {
  sf::Clock clock;
  auto time_since_last_update = sf::Time::Zero;
  while (window_.isOpen()) {
    processEvents();
    time_since_last_update += clock.restart();
    while (time_since_last_update > kTimePerFrame) {
      time_since_last_update -= kTimePerFrame;
      processEvents();
      update(kTimePerFrame);
      render();
    }
  }
}

void Game::processEvents() {
  sf::Event event{};
  while (window_.pollEvent(event)) {
    switch (event.type) {
      case sf::Event::MouseMoved:
        world_.handlePlayerInput(event.mouseMove);
        break;
      case sf::Event::Closed:
        window_.close();
        break;
      default:
        break;
    }
  }
}

void Game::update(const sf::Time elapsed_time) { world_.update(elapsed_time); }

void Game::render() {
  window_.clear();
  world_.draw();
  window_.display();
}

}  // namespace ink
