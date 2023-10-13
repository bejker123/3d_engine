#include "game.hpp"
#include "rendering/buffers.hpp"
#include "rendering/camera.hpp"
#include "rendering/material.hpp"
#include "rendering/mesh.hpp"
#include "rendering/model.hpp"
#include "rendering/vertex_array.hpp"
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <cglm/cglm.h>
#include <cstring>
#include <iostream>
#include <memory>
#include <vector>

std::vector<VertexPC> vertices = {
    VertexPC(glm::vec3(0.5f, 0.5f, 0.0f),
             glm::vec4(0.3f, 1.f, 1.f, 1.f)), // top right
                                              //
    VertexPC(glm::vec3(0.5f, -0.5f, 0.0f),
             glm::vec4(1.f, 1.f, 0.5f, 1.f)), // bottom right
                                              //
    VertexPC(glm::vec3(-0.5f, -0.5f, 0.0f),
             glm::vec4(0.3f, 0.f, 0.5f, 1.f)), // bottom left
                                               //
    VertexPC(glm::vec3(-0.5f, 0.5, 0.0f),
             glm::vec4(0.3f, 1.f, 0.5f, 1.f)) // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

const char *basic_vertex_shader =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vert_pos;\n"
    "layout (location = 1) in vec4 vert_color;\n"
    "out vec4 frag_color;"
    "void main(){\n"
    "frag_color = vert_color;"
    "gl_Position = vec4(vert_pos.x,vert_pos.y,vert_pos.z,1.f);\n"
    "}\0";

const char *camera_vs =
    R"(#version 330
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

const char *basic_fragment_shader = "#version 330 core\n"
                                    "in vec4 frag_color;\n"
                                    "out vec4 FragColor;\n"
                                    "void main(){\n"
                                    // "FragColor = vec4(0.3f,1.f,0.5f,1.f);\n"
                                    "FragColor = frag_color;\n"
                                    "}\0";

Shader shader;

VertexBuffer vb;
VertexBuffer vb1;
IndexBuffer ib;
VertexArray va;
Camera cam;
Model model, model1;

float last_mouse_x = 0, last_mouse_y = 0;

// Init Game State, run the main loop
int Game::initGS(int argc, char *argv[]) {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->inited = false;
  this->monitor = NULL;

  // Run initialisation functions
  this->initCommandLineArgs(argc, argv);

  if (!initOpenGL())
    return EXIT_FAILURE;

  // LOG_VAR("%d", shader.id);
  // LOG_VAR("%d", shader.success);

  cam.init(60, 0.0001, 100000, glm::vec3(1, 0, 1));
  shader.init(camera_vs, basic_fragment_shader, "");

  // LOG_VAR("%d", shader.id);
  // LOG_VAR("%d", shader.success);

  va.init();

  vb.init(vertices);

  ib.init(indices, sizeof(indices));

  va.addVertexBuffer(&vb);
  // va.addVertexBuffer(&vb1);
  va.setIndexBuffer(&ib);

  Mesh mesh;
  mesh.init(std::make_shared<VertexArray>(va));
  // mesh.load(
  //     "/home/bejker/Downloads/Survival_BackPack_2/Survival_BackPack_2.fbx");
  Material mat;
  mat.init(std::make_shared<Shader>(shader));
  model.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  model1.init(std::make_shared<Mesh>(mesh), std::make_unique<Material>(mat));
  // model1.setPos(glm::vec3(1, 0, 1));
  model1.setRot(glm::vec3(90, 0, 0));

  // If all initialisation functions run succesfully
  // set game state to inited
  this->inited = true;

  // Run the main loop
  return runGame();
}

// Parse and init cmd line args
void Game::initCommandLineArgs(int argc, char *argv[]) {
  LOG("INITIALISING Command Line Arguments\n");
  this->options.window_title = (char *)"test";
  this->options.window_width = 800;
  this->options.window_height = 600;
  this->options.window_resizable = false;
  this->options.window_fullscreen = true;
  this->monitor = NULL;
}

// Initialise OpenGL, return true if successful
bool Game::initOpenGL() {
  LOG("INITIALISING OPENGL\n");
  LOG("INITIALISING GLFW\n");
  if (!glfwInit()) {
    LOG("FAILED TO INIT GLFW\n");
    this->terminateOpenGL();
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
    this->terminateOpenGL();
  }
  window.hideCursor();

  /*int frameBufferWidth = 0;
        int frameBufferHeight = 0;

 glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        glfwSetFramebufferSizeCallback(this->window,
 (GLFWframebuffersizefun)framebuffer_resize_callback);
*/

  LOG("INITIALISING GLEW\n");

  glewExperimental = GL_TRUE;

  if (glewInit() != GLEW_OK) {
    LOG("FAILED TO INIT GLEW\n");
    terminateOpenGL();
    return false;
  }

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

void Game::terminateGS() {
  LOG("TERMINATION STARTED\n");

  terminateOpenGL();

  LOG("TERMINATION COMPELTE\n");
}

void Game::terminateOpenGL() {
  LOG("TERMINATING OPENGL\n");
  this->window.~Window();
  glfwTerminate();
}

// Main game loop
int Game::runGame() {
  while (!this->window.shouldClose()) {
    if (!updateGame())
      return EXIT_SUCCESS;

    if (!renderGame())
      return EXIT_SUCCESS;
  }

  terminateGS();

  return EXIT_SUCCESS;
}

// Update loop running every frame,
// before render function,
// handles user input, updates phisics, logic, etc.
int Game::updateGame() {
  this->curTime = static_cast<float>(glfwGetTime());
  this->dt = this->curTime - this->lastTime;
  this->lastTime = this->curTime;
  glfwPollEvents();

  if (!handleKeyboard())
    return 0;

  auto [mouse_x, mouse_y] = this->window.getMousePos();

  cam.updateMosueInput(1, mouse_x - last_mouse_x, last_mouse_y - mouse_y);

  last_mouse_x = mouse_x;
  last_mouse_y = mouse_y;

  model.update();
  model1.update();
  // PRINT_VAR(x);
  return 1;
}

// Handles keyboard user input
// runs every frame
int Game::handleKeyboard() {
  if (this->window.getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
    this->window.setShouldClose(true);

  float cam_speed = 3;

  if (this->window.getKey(GLFW_KEY_W) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * cam.getFront());
  if (this->window.getKey(GLFW_KEY_S) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * -cam.getFront());

  if (this->window.getKey(GLFW_KEY_A) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * -cam.getRight());
  if (this->window.getKey(GLFW_KEY_D) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * cam.getRight());

  if (this->window.getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * cam.getUp());
  if (this->window.getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
    cam.addPos(glm::vec3(cam_speed * this->dt) * -cam.getWorldUp());
  return 1;
}

// Rendering function running every frame, after update
int Game::renderGame() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

  // TODO: Possibly don't use pointers to bind in the future?
  cam.UploadToShader(&shader, &window);
  model.render();
  model1.render();

  this->window.swapBuffers();
  return 1;
}
