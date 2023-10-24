#ifndef __GAME_H
#define __GAME_H
#include "window.hpp"
#include <chrono>

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
  GLFWmonitor *monitor;
  bool inited;

  float delta;
  std::chrono::steady_clock::time_point curr_time, last_time;
  bool paused;
};

#endif //__GAME_H
