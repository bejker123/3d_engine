#pragma once
#include "logic/performence.hpp"
#include "prelude.hpp"
#include "rendering/skybox.hpp"
#include <map>

namespace En {

template <class T> using UUIDmap = std::map<UUID, T>;
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
  UUID add_shader(const char *v, const char *f, const char *g);
  UUID load_shader(const char *v, const char *f, const char *g = "");
  UUID add_va(pVertexArray va);

  UUID add_model(Model &model);
  UUID add_model(pMesh mesh);
  UUID add_model(std::vector<Vertex> vertices,
                 std::vector<unsigned int> indices, pMaterial mat);

  std::optional<Model *> get_model(const UUID idx);
  const size_t get_models_count() const;

  std::optional<pShader> get_shader(const UUID idx) const;
  const size_t get_shaders_count() const;

  // Ids correspond to opengl ids
  std::optional<pVertexArray> get_va(const UUID id) const;
  // main functions

  int run();
  int update();
  int render();
  Camera cam;
  Window window;

private:
  void reload_shaders();

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

  UUIDmap<Model> models;

  // To keep the order in debug ui;
  std::vector<UUID> model_uuids;

  UUIDmap<pMesh> meshes;
  UUIDmap<pShader> shaders;

  UUID active_shader_id;

  UUIDmap<pVertexArray> vas;
  SkyBox skybox;
};
} // namespace En
