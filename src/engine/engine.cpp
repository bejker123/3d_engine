
#include "engine.hpp"
#include "rendering/ll/opengl.hpp"

#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/imgui.h"

#include <memory>
#include <ranges>
#include <thread>

namespace rv = std::ranges::views;

// Init Game State, run the main loop
int Engine::init() {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->state = EngineState::UNINITED;

  // Run initialisation functions
  this->init_command_line_args();

  if (!init_opengl())
    return EXIT_FAILURE;

  // If all initialisation functions run succesfully
  // set game state to inited
  this->state = EngineState::OK;
  this->paused = false;

  this->init_imgui();

  // Run the main loop
  // return run();
  return this->state;
}

vector<int> ms_idxs;
void Engine::add_shader(const char *v, const char *f, const char *g) {
  this->shaders.push_back(shared_ptr<Shader>(new Shader(v, f, g)));
}
void Engine::add_model(Model &model) {
  this->models.push_back(model);
  ms_idxs.push_back(0);
}

optional<shared_ptr<const Shader>> Engine::get_shader(const size_t idx) const {
  if (this->get_shaders_count() - 1 < idx || this->get_shaders_count() == 0)
    return nullopt;
  auto ret = this->shaders.at(idx);
  return optional(ret);
}

const size_t Engine::get_shaders_count() const { return this->shaders.size(); }

// Parse and init cmd line args
void Engine::init_command_line_args() {
  LOG("INITIALISING Command Line Arguments\n");
  this->options.window_title = (char *)"test";
  this->options.window_width = 800;
  this->options.window_height = 600;
  this->options.window_resizable = false;
  this->options.window_fullscreen = true;
}

// Initialise OpenGL, return true if successful
bool Engine::init_opengl() {
  LOG("INITIALISING OPENGL\n");
  LOG("INITIALISING GLFW\n");
  if (!opengl::setup()) {
    LOG("FAILED TO INIT GLFW\n");
    this->terminate_opengl();
    return false;
  }

  LOG("INITIALISING WINDOW\n");

  if (!this->window.init(
          this->options.window_width, this->options.window_height,
          this->options.window_title, this->options.window_resizable,
          this->options.window_fullscreen, true)) {
    this->terminate_opengl();
  }
  window.hide_cursor();

  LOG("INITIALISING GLEW\n");

  glewExperimental = GL_TRUE;

  if (!opengl::setup_glew()) {
    LOG("FAILED TO INIT GLEW\n");
    terminate_opengl();
    return false;
  }

  LOG("OPENGL INITIALISED\n");

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
  LOG("TERMINATION STARTED\n");

  terminate_opengl();

  LOG("TERMINATION COMPELTE\n");
}

void Engine::terminate_opengl() {
  LOG("TERMINATING OPENGL\n");
  this->window.~Window();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  opengl::terminate();
}

// Handles keyboard user input
// runs every frame
int Engine::handle_keyboard() {
  if (this->window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    this->window.set_should_close(true);

  float cam_speed = 10;

  if (this->window.get_key(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    cam_speed = 3;

  if (this->window.get_key(GLFW_KEY_W) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                cam.get_front());
  if (this->window.get_key(GLFW_KEY_S) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_front());

  if (this->window.get_key(GLFW_KEY_A) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_right());
  if (this->window.get_key(GLFW_KEY_D) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                cam.get_right());

  if (this->window.get_key(GLFW_KEY_SPACE) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) * cam.get_up());
  if (this->window.get_key(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->perf.get_delta()) *
                -cam.get_world_up());
  return 1;
}

// Update loop running every frame,
// before render function,
// handles user input, updates phisics, logic, etc.
int Engine::update() {
  this->perf.update();
  glfwPollEvents();
  bool cancel_mouse_delta = false;

  if ((this->window.get_key(GLFW_KEY_TAB) == GLFW_PRESS) &&
      last_tab_pressed == 0) {
    this->paused = !paused;
    if (this->paused)
      this->window.show_cursor();
    else {
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

  for (auto &m : models)
    m.update();

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
    if (ImGui::CollapsingHeader(("models[" + std::to_string(i) + "]").data())) {
      ImGui::BeginGroup();
      if (ImGui::Button("Teleport")) {
        *cam.get_pos() = *models[i].get_pos();
      }
      ImGui::Checkbox("Cull Backfaces",
                      &models[i].get_material()->get_options()->cull_backfaces);
      ImGui::Checkbox("Textured",
                      &models[i].get_material()->get_options()->texture);
      ImGui::RadioButton("Lines",
                         &models[i].get_material()->get_options()->polygon_mode,
                         GL_LINE);
      ImGui::RadioButton("Fill",
                         &models[i].get_material()->get_options()->polygon_mode,
                         GL_FILL);
      ImGui::RadioButton("Point",
                         &models[i].get_material()->get_options()->polygon_mode,
                         GL_POINT);

      ImGui::SliderFloat("Pos X", &models[i].get_pos()->x, -180, 180);
      ImGui::SliderFloat("Pos Y", &models[i].get_pos()->y, -180, 180);
      ImGui::SliderFloat("Pos Z", &models[i].get_pos()->z, -180, 180);

      ImGui::SliderFloat("Rotation X", &models[i].get_rot()->x, -180, 180);
      ImGui::SliderFloat("Rotation Y", &models[i].get_rot()->y, -180, 180);
      ImGui::SliderFloat("Rotation Z", &models[i].get_rot()->z, -180, 180);

      ImGui::EndGroup();
    }
  }
}

// Rendering function running every frame, after update
int Engine::render() {
  opengl::clear_buffer();

  render_imgui();

  // TODO: Possibly don't use pointers to bind in the future?
  cam.upload_to_shader(shaders[0].get(), &window);
  // model.render();
  // model1.render();
  for (auto &m : models) {
    m.render();
  }

  // Rendering
  // (Your code clears your framebuffer, renders your other stuff etc.)
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  // (Your code calls glfwSwapBuffers() etc.)
  this->window.swap_buffers();
  return 1;
}
