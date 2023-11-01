
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "../game.hpp"
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
  int init(int argc, char *argv[]);

private:
  void init_command_line_args(int argc, char *argv[]);
  bool init_opengl();

  // terminate functions
  void terminate();
  void terminate_opengl();

  // main functions

  int run();
  int update();

  // render functions
  int render();
  void render_imgui();

  // update functions
  int handle_keyboard();

  // variables
  WindowOptions options;
  Window window;
  EngineState state;

  Performence perf;

  bool paused;
  float last_mouse_x = 0, last_mouse_y = 0;
  int last_tab_pressed = 0;

  std::vector<Model> models;
  std::vector<Shader> shaders;

  VertexBuffer vb;
  VertexBuffer vb1;
  IndexBuffer ib;
  VertexArray va;
  Camera cam;
  Game game;
};

#endif //__GAME_H
