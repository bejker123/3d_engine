#ifndef __GAME_H
#define __GAME_H

#include "engine/engine.hpp"
using EN = En::Engine *;

class App {
public:
  // FUNCTIONS
  // init functions

  int pre_init(EN engine);
  int init(EN engine);

  int pre_update(EN engine);
  int update(EN engine);
  int post_update(EN engine);

  void pre_render(EN engine);
  void render(EN engine);
  void post_render(EN engine);

  void pre_terminate(EN engine);
  void terminate(EN engine);

private:
  En::ll::VertexBuffer vb;
  En::ll::IndexBuffer ib;
  En::ll::VertexArray va;
};

#endif //__GAME_H
