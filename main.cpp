#include <Game.hpp>

#include <iostream>

#include <SFML/Graphics.hpp>

int main() {
  try {
    ink::Game game;
    game.run();
  } catch (std::exception& e) {
    std::cout << "\nEXCEPTION: " << e.what() << std::endl;
  }
}