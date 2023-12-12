#ifndef __GAME_H
#define __GAME_H

#include "engine/engine.hpp"
#include "engine/rendering/ll/vertex_array.hpp"
class App {
public:
  // FUNCTIONS
  // init functions

  int init(En::Engine *engine);
  int update(En::Engine *engine);
  void render(En::Engine *engine);

private:
  En::ll::VertexBuffer vb;
  En::ll::VertexBuffer vb1;
  En::ll::IndexBuffer ib;
  En::ll::VertexArray va;
};

#endif //__GAME_H
