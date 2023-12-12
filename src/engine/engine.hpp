
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "logic/performence.hpp"
#include "rendering/camera.hpp"
#include "rendering/model.hpp"

#include "window.hpp"

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
  // FUNCTIONS
  // init functions
  int init();
  void add_shader(Shader &shader);
  void add_model(Model &shader);

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
  ~Engine();
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
  std::vector<Shader> shaders;
};

#endif //__GAME_H
