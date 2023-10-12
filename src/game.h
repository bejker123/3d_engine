#ifndef __GAME_H
#define __GAME_H
#include "shader.h"
#include "vertex_array.h"
#include <cglm/cglm.h>

//#define PRINT_VAR(X)\
//        printf(#X" = %d@ %p\n",X,&X)

#define OPENGL_VER_MAJ 3
#define OPENGL_VER_MIN 3

// STRUCTS

// Struct storing game options
struct GameOptions {
  int window_width, window_height;
  char *window_title;
  bool window_resizable, window_fullscreen;
};

typedef struct GameOptions GO;

// Struct storing the whole game data
struct GameState {
  GO options;
  GLFWwindow *window;
  GLFWmonitor *monitor;
  bool inited;
};

typedef struct GameState _GS;

// Define a global Game State
extern _GS GS;

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

#endif //__GAME_H
