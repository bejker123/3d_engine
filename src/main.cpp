#include "game.hpp"

int main(int argc, char **argv) {
  Engine engine;
  engine.init(argc, argv);
  Game game;
  game.init();
}
