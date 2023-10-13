#ifndef __GAME_H
#define __GAME_H
#include "rendering/shader.hpp"
#include "window.hpp"

//#define PRINT_VAR(X)\
//        printf(#X" = %d@ %p\n",X,&X)

#define OPENGL_VER_MAJ 3
#define OPENGL_VER_MIN 3

// STRUCTS

// Struct storing game options
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
  int initGS(int argc, char *argv[]);
  void initCommandLineArgs(int argc, char *argv[]);
  bool initOpenGL();

  // terminate functions
  void terminateGS();
  void terminateOpenGL();

  // main functions

  int runGame();
  int updateGame();
  int renderGame();

  // update functions
  int handleKeyboard();

  // render functions
private:
  GameOptions options;
  Window window;
  GLFWmonitor *monitor;
  bool inited;

  float dt;
  float curTime;
  float lastTime;
};

#endif //__GAME_H
