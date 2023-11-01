
#include "engine.hpp"
#include "rendering/ll/opengl.hpp"
// #include "shaders.hpp"

#include "../../imgui/backends/imgui_impl_glfw.h"
#include "../../imgui/backends/imgui_impl_opengl3.h"
#include "../../imgui/imgui.h"

#include "../stb/stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <iostream>
#include <memory>
#include <ranges>
#include <thread>
const char *basic_vertex_shader =
    "#version 460 core\n"
    "layout (location = 0) in vec3 vert_pos;\n"
    "layout (location = 1) in vec4 vert_color;\n"
    "out vec4 frag_color;"
    "void main(){\n"
    "frag_color = vert_color;"
    "gl_Position = vec4(vert_pos.x,vert_pos.y,vert_pos.z,1.f);\n"
    "}\0";

const char *camera_vs =
    R"(#version 460
layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 vertex_normal;
layout(location = 2) in vec2 vertex_texcoord;
layout(location = 3) in vec4 vertex_color;

out vec3 vs_position;
// out vec3 vs_normal;
out vec2 vs_texcoord;
out vec4 frag_color;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
  // vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
  // vs_position = vertex_position;
  frag_color = vertex_color;
  vs_texcoord = vertex_texcoord;
  // vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
  // vs_normal = mat3(ModelMatrix) * vertex_normal;

  gl_Position =
      ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
      // ProjectionMatrix * ViewMatrix * vec4(vertex_position, 1.f);
}
)";

const char *basic_fs = "#version 460 core\n"
                       "in vec4 frag_color;\n"
                       "in vec2 vs_texcoord;\n"
                       "out vec4 FragColor;\n"
                       "uniform sampler2D tex;"
                       "void main(){\n"
                       // "FragColor = vec4(0.3f,1.f,0.5f,1.f);\n"
                       "FragColor = texture(tex,vs_texcoord)* frag_color;\n"
                       // "FragColor = frag_color;\n"
                       "}\0";

namespace rv = std::ranges::views;
namespace ranges = std::ranges;

std::vector<VertexC> vertices{
    // Front Face 0
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(0.0f, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(1, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 0.0f),
            glm::vec4(1, 1, 0, 1)),

    // Top Fglm::vec3(ace 4
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(1, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(0.0f, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 1),
            glm::vec4(1, 1, 0, 1)),

    // Rightglm::vec3( Face 8
    VertexC(glm::vec3(1, 1, 1), glm::vec3(1), glm::vec2(1, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(1, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(0, 1.0f),
            glm::vec4(1, 1, 0, 1)),

    // Left glm::vec3(Face 12
    VertexC(glm::vec3(-1, 1, 1), glm::vec3(1), glm::vec2(1.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(0, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(1, 0.0f),
            glm::vec4(1, 1, 0, 1)),

    // Bottoglm::vec3(m Face 16
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1.0f, 0.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, 1), glm::vec3(1), glm::vec2(0.0f, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(1, 1),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, 1), glm::vec3(1), glm::vec2(0, 1.0f),
            glm::vec4(1, 1, 0, 1)),

    // Back glm::vec3(Face 20
    VertexC(glm::vec3(-1, 1, -1), glm::vec3(1), glm::vec2(1, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, 1, -1), glm::vec3(1), glm::vec2(0.0f, 1.0f),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(-1, -1, -1), glm::vec3(1), glm::vec2(1, 0),
            glm::vec4(1, 1, 0, 1)),
    VertexC(glm::vec3(1, -1, -1), glm::vec3(1), glm::vec2(0.0f, 0),
            glm::vec4(1, 1, 0, 1)),
};

unsigned int indices[] = {
    0,  1,  3,  3,  1,  2,  // Front face
    7,  5,  6,  6,  5,  4,  // Top Face
    8,  9,  11, 11, 9,  10, // Right face
    12, 13, 15, 15, 13, 14, // Left face
    19, 17, 18, 18, 17, 16, // Bottom face
    20, 21, 22, 22, 21, 23  // Back face
};

// Init Game State, run the main loop
int Engine::init(int argc, char *argv[]) {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->state = EngineState::UNINITED;

  // Run initialisation functions
  this->init_command_line_args(argc, argv);

  if (!init_opengl())
    return EXIT_FAILURE;

  cam.init(60, 0.0001, 100000, glm::vec3(-40, 20, 30));
  Shader shader;
  shader.init(camera_vs, basic_fs, "");
  shaders.push_back(shader);

  va.init();

  vb.init(vertices);

  ib.init(indices, sizeof(indices));

  va.add_vertex_buffer(&vb);
  // va.addVertexBuffer(&vb1);
  va.set_index_buffer(&ib);

  Mesh mesh;
  mesh.init(std::make_shared<VertexArray>(va));
  // mesh.load(
  //     "/home/bejker/Downloads/Survival_BackPack_2/Survival_BackPack_2.fbx");
  Material mat;
  mat.init(std::make_shared<Shader>(shader));
  // TODO: Change this texture
  Texture tex("face.png");
  mat.set_texture(std::make_shared<Texture>(tex));
  // model.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  // model1.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  // // model1.setPos(glm::vec3(1, 0, 1));
  // model1.setRot(glm::vec3(90, 0, 0));
  for (uint64_t i : rv::iota(0, 10)) {
    Model m;
    m.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
    m.set_origin(glm::vec3((double)i * 21, 0, 0));
    models.push_back(m);
  }

  // If all initialisation functions run succesfully
  // set game state to inited
  this->state = EngineState::OK;
  this->paused = false;

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

  // Run the main loop
  return run();
}

// Parse and init cmd line args
void Engine::init_command_line_args(int argc, char *argv[]) {
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
          this->options.window_fullscreen)) {
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

void Engine::terminate() {
  LOG("TERMINATION STARTED\n");

  this->cam.~Camera();
  terminate_opengl();

  LOG("TERMINATION COMPELTE\n");
}

void Engine::terminate_opengl() {
  LOG("TERMINATING OPENGL\n");
  this->window.~Window();
  for (auto &i : this->shaders)
    i.~Shader();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  opengl::terminate();
}

// Main game loop
int Engine::run() {
  while (!this->window.should_close()) {
    if (!update())
      return EXIT_SUCCESS;

    if (!render())
      return EXIT_SUCCESS;
  }

  terminate();

  return EXIT_SUCCESS;
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
  cam.upload_to_shader(&shaders[0], &window);
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
