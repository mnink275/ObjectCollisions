#include <Game.hpp>

namespace ink {

Game::Game()
    : window_(sf::VideoMode({1280, 720}), "ObjectCollisions", sf::Style::Close),
      world_(window_) {
  window_.setPosition({0, 0});
}

void Game::run() {
  sf::Clock clock;
  sf::Time time_since_last_update = sf::Time::Zero;
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
      // unused
      case sf::Event::KeyPressed:
      case sf::Event::KeyReleased:
      case sf::Event::Resized:
      case sf::Event::LostFocus:
      case sf::Event::GainedFocus:
      case sf::Event::TextEntered:
      case sf::Event::MouseWheelScrolled:
      case sf::Event::MouseButtonPressed:
      case sf::Event::MouseButtonReleased:
      case sf::Event::MouseEntered:
      case sf::Event::MouseLeft:
      case sf::Event::JoystickButtonPressed:
      case sf::Event::JoystickButtonReleased:
      case sf::Event::JoystickMoved:
      case sf::Event::JoystickConnected:
      case sf::Event::JoystickDisconnected:
      case sf::Event::TouchBegan:
      case sf::Event::TouchMoved:
      case sf::Event::TouchEnded:
      case sf::Event::SensorChanged:
      case sf::Event::Count:
        break;
    }
  }
}

void Game::update(const sf::Time elapsed_time) { world_.update(elapsed_time); }

void Game::render() {
  window_.clear();
  world_.draw();

  window_.setView(window_.getDefaultView());
  window_.display();
}

}  // namespace ink
