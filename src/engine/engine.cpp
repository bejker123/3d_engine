#include "engine.hpp"
#include "rendering/ll/opengl.hpp"

#include "../app.hpp"
#include "io/logger.hpp"
#include <ranges>
#include <stdexcept>
#include <thread>

#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/imgui.h"
#include "io/shader_loader.hpp"

std::unique_ptr<App> app;
namespace rv = std::ranges::views;
namespace En {

// Init Game State, run the main loop
Engine::Engine() {
  LOG("[ENGINE] INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->state = EngineState::UNINITED;

  // Run initialisation functions
  this->init_command_line_args();
  app = std::unique_ptr<App>(new App());
  app->pre_init(this);

  if (!init_opengl())
    return; // EXIT_FAILURE

  this->keyboard = Keyboard(this->window.raw());

  // If all initialisation functions run succesfully
  // set game state to inited
  this->state = EngineState::OK;
  this->paused = false;

  this->init_imgui();
  auto x = std::make_shared<Shader>(
      ShaderLoader::load("shaders/sb_vertex.glsl", "shaders/sb_fragment.glsl")
          .value());

  std::vector<std::string> faces = {
      "textures/skybox/right.jpg", "textures/skybox/left.jpg",
      "textures/skybox/top.jpg",   "textures/skybox/bottom.jpg",
      "textures/skybox/front.jpg", "textures/skybox/back.jpg"};
  this->skybox.init(x, faces);
  //
  // Run the main loop
  // return run();
  app->init(this);
  // return this->state;
}

std::vector<int> ms_idxs;
void Engine::add_shader(const char *v, const char *f, const char *g) {
  this->shaders.push_back(pShader(new Shader(v, f, g)));
}

void Engine::load_shader(const char *v, const char *f, const char *g) {
  this->shaders.push_back(
      std::make_shared<Shader>(ShaderLoader::load(v, f, g).value()));
}

void Engine::add_va(pVertexArray va) {
  if (this->vas.contains(va->get_id()))
    return;
  this->vas.insert(std::make_pair(va->get_id(), va));
}

void Engine::add_model(Model &model) {
  this->models.push_back(model);
  ms_idxs.push_back(0);
}

void Engine::add_model(pMesh mesh) {
  this->models.push_back(Model(mesh));
  ms_idxs.push_back(0);
}

void Engine::add_model(std::vector<Vertex> vertices,
                       std::vector<unsigned int> indices, pMaterial mat) {
  this->meshes.push_back(std::make_shared<Mesh>(Mesh(mat, vertices, indices)));
  this->models.push_back(Model(this->meshes.at(this->meshes.size() - 1)));
  ms_idxs.push_back(0);
}

std::optional<Model *> Engine::get_model(const uint32_t idx) {
  try {
    return std::optional(&this->models.at(idx));
  } catch (std::out_of_range e) {
    return std::nullopt;
  }
}

std::optional<Model *> Engine::get_last_model() {
  try {
    return std::optional(&this->models.at(this->models.size() - 1));
  } catch (std::exception e) {
    return std::nullopt;
  }
}

const size_t Engine::get_models_count() const { return this->models.size(); }

std::optional<pShader> Engine::get_shader(const size_t idx) const {
  if (this->get_shaders_count() - 1 < idx || this->get_shaders_count() == 0)
    return std::nullopt;
  auto ret = this->shaders.at(idx);
  return std::optional(ret);
}

const size_t Engine::get_shaders_count() const { return this->shaders.size(); }

void Engine::reload_shaders() {
  for (auto &i : this->shaders) {
    auto new_ = ShaderLoader::reload(i->get_id());
    if (new_.has_value()) {
      i->terminate();
      *i = new_.value();
    } else {
      LOG("[ENGINE] Shader %d not reloaded\n", i->get_id());
    }
  }
}

std::optional<pVertexArray> Engine::get_va(const uint32_t id) const {
  try {
    return std::optional(this->vas.at(id));
  } catch (std::out_of_range e) {
    return std::nullopt;
  }
}

// Parse and init cmd line args
void Engine::init_command_line_args() {
  LOG("[ENGINE] INITIALISING Command Line Arguments\n");
  this->options.window_title = (char *)"test";
  this->options.window_width = 800;
  this->options.window_height = 600;
  this->options.window_resizable = true;
  this->options.window_fullscreen = false;
}

// Initialise OpenGL, return true if successful
bool Engine::init_opengl() {
  LOG("[ENGINE] INITIALISING OPENGL\n");
  LOG("[ENGINE] INITIALISING GLFW\n");
  if (!opengl::setup()) {
    LOG("[ENGINE] FAILED TO INIT GLFW\n");
    this->terminate_opengl();
    return false;
  }

  LOG("[ENGINE] INITIALISING WINDOW\n");

  if (!this->window.init(
          this->options.window_width, this->options.window_height,
          this->options.window_title, this->options.window_resizable,
          this->options.window_fullscreen, true)) {
    this->terminate_opengl();
  }
  window.hide_cursor();

  LOG("[ENGINE] INITIALISING GLEW\n");

  if (!opengl::setup_glew()) {
    LOG("[ENGINE] FAILED TO INIT GLEW\n");
    terminate_opengl();
    return false;
  }

  LOG("[ENGINE] OPENGL INITIALISED\n");

  opengl::debug_info();
  return true;
}

void Engine::init_imgui() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  // ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  // io.ConfigFlags |=
  //     ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
  // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // IF using Docking
  // Branch

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(
      this->window.raw(),
      true); // Second param install_callback=true will install
             // GLFW callbacks and chain to existing ones.
  ImGui_ImplOpenGL3_Init();
}

Engine::~Engine() {
  LOG("[ENGINE] TERMINATION STARTED\n");

  app->pre_terminate(this);
  terminate_opengl();
  app->terminate(this);

  LOG("[ENGINE] TERMINATION COMPELTE\n");
}

void Engine::terminate_opengl() {
  LOG("[ENGINE] TERMINATING OPENGL\n");
  this->window.~Window();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  for (auto &i : this->shaders) {
    i->terminate();
  }
  for (auto &i : this->models) {
    for (auto &j : i.get_meshes()) {
      auto tex = j->get_material()->get_texture();
      if (tex.has_value())
        tex.value()->terminate();
    }
  }
  opengl::terminate();
}

// Handles keyboard user input
// runs every frame
int Engine::handle_keyboard() {
  if (this->keyboard.get_key_pressed(Key::Code::KEY_ESCAPE))
    this->window.set_should_close(true);

  float cam_speed = 20;

  if (!this->keyboard.get_key_released(Key::Code::KEY_LEFT_SHIFT))
    cam_speed = 40;

  if (!this->keyboard.get_key_released(Key::Code::KEY_W))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                cam.get_front());
  if (!this->keyboard.get_key_released(Key::Code::KEY_S))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_front());

  if (!this->keyboard.get_key_released(Key::Code::KEY_A))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_right());
  if (!this->keyboard.get_key_released(Key::Code::KEY_D))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                cam.get_right());

  if (!this->keyboard.get_key_released(Key::Code::KEY_SPACE))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                cam.get_world_up());
  if (!this->keyboard.get_key_released(Key::Code::KEY_LEFT_CONTROL))
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_world_up());
  return 1;
}

int Engine::run() {
  while (this->update() && !this->window.should_close())
    this->render();
  return 0;
}

// Update loop running every frame,
// before render function,
// handles user input, updates phisics, logic, etc.
int Engine::update() {
  app->pre_update(this);
  this->perf.update();
  glfwPollEvents();
  bool cancel_mouse_delta = false;

  if ((this->keyboard.get_key_pressed(Key::Code::KEY_TAB)) &&
      last_tab_pressed == 0) {
    this->paused = !paused;
    if (this->paused) {
      this->window.show_cursor();
    } else {
      ImGui::SetWindowFocus(nullptr);
      this->window.hide_cursor();
      cancel_mouse_delta = true;
    }
    last_tab_pressed = 0.1 / this->perf.get_delta();
  } else {
    if (last_tab_pressed > 0)
      last_tab_pressed--;
  }
  if (paused)
    return 1;

  if (!handle_keyboard())
    return 0;

  auto [mouse_x, mouse_y] = this->window.get_mouse_pos();

  if (!paused)
    cam.update_mosue_input(1, cancel_mouse_delta ? 0 : mouse_x - last_mouse_x,
                           cancel_mouse_delta ? 0 : last_mouse_y - mouse_y);

  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  app->update(this);

  for (auto &m : models)
    m.update();

  app->post_update(this);

  return 1;
}

void Engine::render_imgui() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Text("Delta: %fms FPS: %ld", this->perf.get_delta() * 1000,
              this->perf.get_fps());
  if (!paused)
    return;

  if (ImGui::Button("Reload Shaders")) {
    this->reload_shaders();
  }
  if (ImGui::CollapsingHeader("Camera")) {
    ImGui::BeginGroup();
    ImGui::SliderFloat("Camera X", &cam.get_pos()->x, -10.0f, 10.0f);
    ImGui::SliderFloat("Camera Y", &cam.get_pos()->y, -10.0f, 10.0f);
    ImGui::SliderFloat("Camera Z", &cam.get_pos()->z, -10.0f, 10.0f);
    ImGui::SliderFloat("Camera FOV", cam.get_fov(), 0, 180);
    ImGui::SliderFloat("Camera pitch", cam.get_pitch(), -90, 90);
    ImGui::SliderFloat("Camera yaw", cam.get_yaw(), 0, 180);
    ImGui::SliderFloat("Camera roll", cam.get_roll(), 0, 180);
    ImGui::EndGroup();
  }

  for (auto i : rv::iota((uint64_t)0, models.size())) {
    ImGui::BeginGroup();
    if (ImGui::CollapsingHeader(("models[" + std::to_string(i) + "]").data())) {
      if (ImGui::Button(("Teleport " + std::to_string(i)).data())) {
        *cam.get_pos() = *models[i].get_pos();
      }

      for (auto j : rv::iota((size_t)0, models[i].get_meshes().size())) {

        ImGui::Checkbox(
            ("Cull Backfaces " + std::to_string(i) + " " + std::to_string(j))
                .data(),
            &models[i]
                 .get_meshes()[j]
                 ->get_material()
                 ->get_options()
                 ->cull_backfaces);
        ImGui::Checkbox(
            ("Textured " + std::to_string(i) + " " + std::to_string(j)).data(),
            &models[i].get_meshes()[j]->get_material()->get_options()->texture);
        ImGui::RadioButton(
            ("Lines " + std::to_string(i) + " " + std::to_string(j)).data(),
            &models[i]
                 .get_meshes()[j]
                 ->get_material()
                 ->get_options()
                 ->polygon_mode,
            GL_LINE);
        ImGui::RadioButton(
            ("Fill " + std::to_string(i) + " " + std::to_string(j)).data(),
            &models[i]
                 .get_meshes()[j]
                 ->get_material()
                 ->get_options()
                 ->polygon_mode,
            GL_FILL);
        ImGui::RadioButton(
            ("Point" + std::to_string(i) + " " + std::to_string(j)).data(),
            &models[i]
                 .get_meshes()[j]
                 ->get_material()
                 ->get_options()
                 ->polygon_mode,
            GL_POINT);
      }

      ImGui::SliderFloat(("Pos X " + std::to_string(i)).data(),
                         &models[i].get_pos()->x, -180, 180);
      ImGui::SliderFloat(("Pos Y " + std::to_string(i)).data(),
                         &models[i].get_pos()->y, -180, 180);
      ImGui::SliderFloat(("Pos Z " + std::to_string(i)).data(),
                         &models[i].get_pos()->z, -180, 180);

      ImGui::SliderFloat(("Rotation X " + std::to_string(i)).data(),
                         &models[i].get_rot()->x, -180, 180);
      ImGui::SliderFloat(("Rotation Y " + std::to_string(i)).data(),
                         &models[i].get_rot()->y, -180, 180);
      ImGui::SliderFloat(("Rotation Z " + std::to_string(i)).data(),
                         &models[i].get_rot()->z, -180, 180);
    }
    ImGui::EndGroup();
  }
}

// Rendering function running every frame, after update
int Engine::render() {
  app->pre_render(this);
  opengl::clear_buffer();

  render_imgui();

  // TODO: Possibly don't use pointers to bind in the future?
  cam.upload_to_shader(shaders[0], &window);
  cam.upload_to_sb_shader(this->skybox.get_shader(), &window);
  this->skybox.render();
  // model.render();
  // model1.render();
  for (auto &m : models) {
    m.render();
  }

  app->render(this);
  // Rendering
  // (Your code clears your framebuffer, renders your other stuff etc.)
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // (Your code calls glfwSwapBuffers() etc.)
  this->window.swap_buffers();
  app->post_render(this);
  return 1;
}

} // namespace En
