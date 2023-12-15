
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "logic/performence.hpp"
#include "prelude.hpp"
#include <map>

namespace En {
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
  Engine();
  ~Engine();
  // FUNCTIONS
  // init functions
  void add_shader(const char *v, const char *f, const char *g);
  void add_va(pVertexArray va);

  void add_model(Model &model);
  void add_model(pMesh mesh);
  void add_model(std::vector<ll::Vertex> vertices,
                 std::vector<unsigned int> indices, pMaterial mat);

  std::optional<Model *> get_model(const uint32_t idx);
  std::optional<Model *> get_last_model();
  const size_t get_models_count() const;

  std::optional<pShader> get_shader(const size_t idx) const;
  const size_t get_shaders_count() const;

  // Ids correspond to opengl ids
  std::optional<pVertexArray> get_va(const uint32_t id) const;
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
  Keyboard keyboard;

  bool paused;
  float last_mouse_x = 0, last_mouse_y = 0;
  int last_tab_pressed = 0;

  std::vector<Model> models;
  std::vector<pMesh> meshes;
  std::vector<pShader> shaders;
  std::map<uint32_t, pVertexArray> vas;
};
} // namespace En

#endif //__GAME_H
