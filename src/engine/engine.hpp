
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "logic/performence.hpp"
#include "rendering/camera.hpp"
#include "rendering/model.hpp"

#include "window.hpp"
#include <memory>

class WindowOptions {
public:
  int window_width, window_height;
  char *window_title;
  bool window_resizable, window_fullscreen;
};

enum EngineState {
  UNINITED,
  OK,
  ERR,
};

class Engine {
public:
  ~Engine();
  // FUNCTIONS
  // init functions
  int init();
  void add_shader(const char *v, const char *f, const char *g);
  void add_model(Model &shader);
  optional<shared_ptr<Shader const>> get_shader(const size_t idx) const;
  const size_t get_shaders_count() const;

  // main functions

  int run();
  int update();
  int render();
  Camera cam;
  Window window;

private:
  void init_command_line_args();
  bool init_opengl();
  void init_imgui();

  // terminate functions
  void terminate_opengl();

  // render functions
  void render_imgui();

  // update functions
  int handle_keyboard();

  // variables
  WindowOptions options;
  EngineState state;

  Performence perf;

  bool paused;
  float last_mouse_x = 0, last_mouse_y = 0;
  int last_tab_pressed = 0;

  std::vector<Model> models;
  std::vector<shared_ptr<Shader>> shaders;
};

#endif //__GAME_H
