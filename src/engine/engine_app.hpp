#include "engine.hpp"
class EngineApp {
public:
  virtual int init(Engine *engine);
  virtual int update(Engine *engine);
  virtual void render(Engine *engine);
};
