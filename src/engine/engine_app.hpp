#include "engine.hpp"
class EngineApp {
public:
  virtual int init(En::Engine *engine);
  virtual int update(En::Engine *engine);
  virtual void render(En::Engine *engine);
};
