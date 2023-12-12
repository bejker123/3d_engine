#ifndef __GAME_H
#define __GAME_H

#include "engine/engine.hpp"
#include "engine/rendering/ll/vertex_array.hpp"
class App {
public:
  // FUNCTIONS
  // init functions

  int init(Engine *engine);
  int update(Engine *engine);
  void render(Engine *engine);

private:
};

#endif //__GAME_H
