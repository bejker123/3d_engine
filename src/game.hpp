#ifndef __GAME_H
#define __GAME_H
#include "window.hpp"

#define OPENGL_VER_MAJ 3
#define OPENGL_VER_MIN 3

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
  void init_command_line_args(int argc, char *argv[]);
  bool init_opengl();

  // terminate functions
  void terminate();
  void terminate_opengl();

  // main functions

  int run();
  int update();
  int render();

  // update functions
  int handle_keyboard();

  // render functions
private:
  GameOptions options;
  Window window;
  GLFWmonitor *monitor;
  bool inited;

  float dt;
  float curTime;
  float lastTime;
  bool paused;
};

#endif //__GAME_H
