#ifndef __GAME_H
#define __GAME_H
#include "engine/engine.hpp"

class Game {
public:
  // FUNCTIONS
  // init functions

  Game();

  int update();
  void render();

private:
  std::unique_ptr<Engine> engine;

  VertexBuffer vb;
  VertexBuffer vb1;
  IndexBuffer ib;
  VertexArray va;
};

#endif //__GAME_H
