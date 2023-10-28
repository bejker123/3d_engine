#ifndef __GAME_H
#define __GAME_H
#include "logic/performence.hpp"
#include "rendering/camera.hpp"
#include "rendering/model.hpp"
#include "window.hpp"

#define OPENGL_VER_MAJ 4
#define OPENGL_VER_MIN 6

class GameOptions {
public:
  int window_width, window_height;
  char *window_title;
  bool window_resizable, window_fullscreen;
};

class Game {
public:
  // FUNCTIONS
  // init functions
  int init(int argc, char *argv[]);

  // render functions
private:
  void init_command_line_args(int argc, char *argv[]);
  bool init_opengl();

  // terminate functions
  void terminate();
  void terminate_opengl();

  // main functions

  int run();
  int update();
  int render();
  void render_imgui();

  // update functions
  int handle_keyboard();
  GameOptions options;
  Window window;
  bool inited;

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
};

#endif //__GAME_H
