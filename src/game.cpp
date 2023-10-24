#include "game.hpp"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "../imgui/imgui.h"
#include "rendering/buffers.hpp"
#include "rendering/camera.hpp"
#include "rendering/material.hpp"
#include "rendering/mesh.hpp"
#include "rendering/model.hpp"
#include "rendering/vertex_array.hpp"
#include "stb/stb_image.h"
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cglm/cglm.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

std::vector<VertexPC> vertices{
    // Front Face 0
    VertexPC(-1, 1, 1, 0.0f, 0.0f, 0, 1),
    VertexPC(-1, -1, 1, 0.0f, 1, 0, 1),
    VertexPC(1, -1, 1, 1, 1, 0, 1),
    VertexPC(1, 1, 1, 1, 0.0f, 0, 1),

    // Top Face 4
    VertexPC(-1, 1, -1, 0.0f, 0.0f, 0, 1),
    VertexPC(1, 1, -1, 1, 0.0f, 0, 1),
    VertexPC(-1, 1, 1, 0.0f, 1, 0, 1),
    VertexPC(1, 1, 1, 1, 1, 0, 1),

    // Right Face 8
    VertexPC(1, 1, 1, 1, 1, 0, 1),
    VertexPC(1, -1, 1, 1, 0, 0, 1),
    VertexPC(1, -1, -1, 0.0f, 0.0f, 0, 1),
    VertexPC(1, 1, -1, 0, 1.0f, 0, 1),

    // Left Face 12
    VertexPC(-1, 1, 1, 1.0f, 1.0f, 0, 1),
    VertexPC(-1, 1, -1, 0.0f, 1, 0, 1),
    VertexPC(-1, -1, -1, 0, 0, 0, 1),
    VertexPC(-1, -1, 1, 1, 0.0f, 0, 1),

    // Bottom Face 16
    VertexPC(-1, -1, -1, 1.0f, 0.0f, 0, 1),
    VertexPC(-1, -1, 1, 0.0f, 0, 0, 1),
    VertexPC(1, -1, -1, 1, 1, 0, 1),
    VertexPC(1, -1, 1, 0, 1.0f, 0, 1),

    // Back Face 20
    VertexPC(-1, 1, -1, 1, 1.0f, 0, 1),
    VertexPC(1, 1, -1, 0.0f, 1.0f, 0, 1),
    VertexPC(-1, -1, -1, 1, 0, 0, 1),
    VertexPC(1, -1, -1, 0.0f, 0, 0, 1),
};

unsigned int indices[] = {
    0,  1,  3,  3,  1,  2,  // Front face
    7,  5,  6,  6,  5,  4,  // Top Face
    8,  9,  11, 11, 9,  10, // Right face
    12, 13, 15, 15, 13, 14, // Left face
    19, 17, 18, 18, 17, 16, // Bottom face
    20, 21, 22, 22, 21, 23  // Back face
};

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
layout(location = 1) in vec4 vertex_color;
 // layout(location = 2) in vec2 vertex_texcoord;
// layout(location = 3) in vec3 vertex_normal;

out vec3 vs_position;
out vec4 frag_color;
// out vec2 vs_texcoord;
// out vec3 vs_normal;

uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
  vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
  // vs_position = vertex_position;
  frag_color = vertex_color;
  // vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
  // vs_normal = mat3(ModelMatrix) * vertex_normal;

  gl_Position =
      ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
      // ProjectionMatrix * ViewMatrix * vec4(vertex_position, 1.f);
}
)";

const char *basic_fragment_shader =
    "#version 460 core\n"
    "in vec4 frag_color;\n"
    // "in vec2 vs_texcoord;\n"
    "out vec4 FragColor;\n"
    "uniform sampler2D tex;"
    "void main(){\n"
    // "FragColor = vec4(0.3f,1.f,0.5f,1.f);\n"
    "FragColor = texture(tex,frag_color.xy) * vec4(1,1,1,0.5);\n"
    // "FragColor = frag_color;\n"
    "}\0";

Shader shader;

VertexBuffer vb;
VertexBuffer vb1;
IndexBuffer ib;
VertexArray va;
Camera cam;
std::vector<Model> models;

float last_mouse_x = 0, last_mouse_y = 0;

// Init Game State, run the main loop
int Game::init(int argc, char *argv[]) {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->inited = false;
  this->monitor = NULL;

  // Run initialisation functions
  this->init_command_line_args(argc, argv);

  if (!init_opengl())
    return EXIT_FAILURE;

  cam.init(60, 0.0001, 100000, glm::vec3(1, 0, 1));
  shader.init(camera_vs, basic_fragment_shader, "");

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
  Texture tex("/run/media/bejker/DATA/C++ OpenGL Game Engine/C++ OpenGL Game "
              "Engine/Images/cat.png");
  mat.set_texture(std::make_shared<Texture>(tex));
  // model.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  // model1.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  // // model1.setPos(glm::vec3(1, 0, 1));
  // model1.setRot(glm::vec3(90, 0, 0));
  for (int i = 0; i < 10; i++) {
    Model m;
    m.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
    m.set_origin(glm::vec3(i * 21, 0, 0));
    models.push_back(m);
  }

  // If all initialisation functions run succesfully
  // set game state to inited
  this->inited = true;
  this->paused = false;

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
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
void Game::init_command_line_args(int argc, char *argv[]) {
  LOG("INITIALISING Command Line Arguments\n");
  this->options.window_title = (char *)"test";
  this->options.window_width = 800;
  this->options.window_height = 600;
  this->options.window_resizable = false;
  this->options.window_fullscreen = true;
  this->monitor = NULL;
}

// Initialise OpenGL, return true if successful
bool Game::init_opengl() {
  LOG("INITIALISING OPENGL\n");
  LOG("INITIALISING GLFW\n");
  if (!glfwInit()) {
    LOG("FAILED TO INIT GLFW\n");
    this->terminate_opengl();
    return false;
  }

  LOG("INITIALISING WINDOW\n");

  // set opengl version to 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJ);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MIN);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  if (!this->window.init(
          this->options.window_width, this->options.window_height,
          this->options.window_title, this->options.window_resizable,
          this->options.window_fullscreen)) {
    this->terminate_opengl();
  }
  window.hide_cursor();

  LOG("INITIALISING GLEW\n");

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    LOG("FAILED TO INIT GLEW\n");
    terminate_opengl();
    return false;
  }

  glDepthFunc(GL_LESS);
  glEnable(GL_LINE_SMOOTH);
  LOG("OPENGL INITIALISED\n");
  /*
      glEnable(GL_DEPTH_TEST);
                  glDepthFunc(GL_ALWAYS);

                  //TODO: see if antialiasing impacts the performence.
                  glfwWindowHint(GLFW_SAMPLES, 4);
                  glEnable(GL_MULTISAMPLE);

                  glEnable(GL_CULL_FACE);
                  glCullFace(GL_BACK);//use this?
                  glFrontFace(GL_CCW);
      */

  LOG("OPENGL DEBUG INFO:\n");
  const GLubyte *vendor = glGetString(GL_VENDOR);     // get renderer string
  const GLubyte *renderer = glGetString(GL_RENDERER); // get renderer string
  const GLubyte *version = glGetString(GL_VERSION);   // version as a string
  LOG("\tGraphics card vendor: %s\n", vendor);
  LOG("\tGraphics card: %s\n", renderer);
  LOG("\tOpenGL version: %s\n", version);

  return true;
}

void Game::terminate() {
  LOG("TERMINATION STARTED\n");

  terminate_opengl();
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  LOG("TERMINATION COMPELTE\n");
}

void Game::terminate_opengl() {
  LOG("TERMINATING OPENGL\n");
  this->window.~Window();
  glfwTerminate();
}

// Main game loop
int Game::run() {
  while (!this->window.should_close()) {
    if (!update())
      return EXIT_SUCCESS;

    if (!render())
      return EXIT_SUCCESS;
  }

  terminate();

  return EXIT_SUCCESS;
}

int last_tab_pressed = 0;

// Handles keyboard user input
// runs every frame
int Game::handle_keyboard() {
  if (this->window.get_key(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    this->window.set_should_close(true);

  float cam_speed = 10;

  if (this->window.get_key(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    cam_speed = 3;

  if (this->window.get_key(GLFW_KEY_W) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * cam.get_front());
  if (this->window.get_key(GLFW_KEY_S) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * -cam.get_front());

  if (this->window.get_key(GLFW_KEY_A) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * -cam.get_right());
  if (this->window.get_key(GLFW_KEY_D) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * cam.get_right());

  if (this->window.get_key(GLFW_KEY_SPACE) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * cam.get_up());
  if (this->window.get_key(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    cam.add_pos(glm::vec3(cam_speed * this->dt) * -cam.get_world_up());
  if ((this->window.get_key(GLFW_KEY_TAB) == GLFW_PRESS) &&
      last_tab_pressed == 0) {
    this->paused = !paused;
    if (this->paused)
      this->window.show_cursor();
    else
      this->window.hide_cursor();
    last_tab_pressed = 5;
  } else {
    if (last_tab_pressed > 0)
      last_tab_pressed--;
  }
  return 1;
}

// Update loop running every frame,
// before render function,
// handles user input, updates phisics, logic, etc.
int Game::update() {
  this->curTime = static_cast<float>(glfwGetTime());
  this->dt = this->curTime - this->lastTime;
  this->lastTime = this->curTime;
  glfwPollEvents();

  if (!handle_keyboard())
    return 0;

  auto [mouse_x, mouse_y] = this->window.get_mouse_pos();

  if (!paused)
    cam.update_mosue_input(1, mouse_x - last_mouse_x, last_mouse_y - mouse_y);

  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  // model.update();
  // model1.update();
  for (auto &m : models) {
    m.update();
  }
  return 1;
}

void Game::render_imgui() {
  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  ImGui::Text("Delta: %f FPS: %d", this->dt, (int)(1 / this->dt));

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

  for (int i = 0; i < models.size(); i++) {
    if (ImGui::CollapsingHeader(("models[" + std::to_string(i) + "]").data())) {
      ImGui::BeginGroup();
      if (ImGui::Button("Teleport")) {
        *cam.get_pos() = *models[i].get_pos();
      }
      ImGui::Checkbox("Cull Backfaces",
                      &models[i].get_material()->get_options()->cull_backfaces);
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
int Game::render() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  render_imgui();

  // TODO: Possibly don't use pointers to bind in the future?
  cam.upload_to_shader(&shader, &window);
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
