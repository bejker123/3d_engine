#include "game.hpp"
#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

float vertices[] = {
    0.5f * 10,  0.5f * 10,  0.0f, // top right
    0.5f * 10,  -0.5f * 10, 0.0f, // bottom right
    -0.5f * 10, -0.5f * 10, 0.0f, // bottom left
    -0.5f * 10, 0.5f * 10,  0.0f  // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

float colors[] = {
    0.3f, 1.f, 1.f,  1.f, 1.f,  1.f, 0.5f, 1.f,
    0.3f, 0.f, 0.5f, 1.f, 0.3f, 1.f, 0.5f, 1.f,
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

// uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main() {
  // vs_position = vec4(ModelMatrix * vec4(vertex_position, 1.f)).xyz;
  vs_position = vertex_position;
  frag_color = vertex_color;
  // vs_texcoord = vec2(vertex_texcoord.x, vertex_texcoord.y * -1.f);
  // vs_normal = mat3(ModelMatrix) * vertex_normal;

  gl_Position =
      // ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(vertex_position, 1.f);
      ProjectionMatrix * ViewMatrix * vec4(vertex_position, 1.f);
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

// Init Game State, run the main loop
int Game::initGS(int argc, char *argv[]) {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  this->inited = false;
  this->window = NULL;
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

  vb.init(vertices, sizeof(vertices), 3 * sizeof(float));

  vb1.init(colors, sizeof(colors), 4 * sizeof(float));

  ib.init(indices, sizeof(indices));

  va.addVertexBuffer(&vb);
  va.addVertexBuffer(&vb1);
  va.setIndexBuffer(&ib);

  LOG_VAR("%d", vb.id);
  LOG_VAR("%d", vb.count);
  LOG_VAR("%d", ib.elements);
  LOG_VAR("%d", ib.id);
  // LOG_VAR("%d", va.getID());
  // LOG_VAR("%p",va.ib);

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
  this->options.window_fullscreen = false;
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

  printf("%p\n", this->monitor);
  if (this->options.window_fullscreen) {
    this->monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(this->monitor, 0, 0, &this->options.window_width,
                           &this->options.window_height);
  }

  // set opengl version to 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJ);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MIN);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, this->options.window_resizable);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  this->window =
      glfwCreateWindow(this->options.window_width, this->options.window_height,
                       this->options.window_title, this->monitor, NULL);

  if (this->window == NULL) {
    LOG("FAILED TO INIT WINDOW\n");
    const char **x = (const char **)malloc(4);
    x[0] = (const char *)malloc(1024);
    glfwGetError(x);
    LOG(x[0]);
    printf("\n");
    terminateOpenGL();
    return false;
  }

  /*int frameBufferWidth = 0;
        int frameBufferHeight = 0;

 glfwGetFramebufferSize(this->window, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        glfwSetFramebufferSizeCallback(this->window,
 (GLFWframebuffersizefun)framebuffer_resize_callback);
*/

  LOG("INITIALISING GLEW\n");

  glfwMakeContextCurrent(this->window);

  glewExperimental = GL_TRUE;

  if (glfwGetCurrentContext() != this->window) {
    LOG("FAIELD TO INIT WINDOW\n");
    terminateOpenGL();
  }

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
  glfwDestroyWindow(this->window);
  glfwTerminate();
}

// Main game loop
int Game::runGame() {
  while (!glfwWindowShouldClose(this->window)) {
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
  glfwPollEvents();

  if (!handleKeyboard())
    return 0;
  // PRINT_VAR(x);
  return 1;
}

// Handles keyboard user input
// runs every frame
int Game::handleKeyboard() {
  if (glfwGetKey(this->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(this->window, true);

  return 1;
}

// Rendering function running every frame, after update
int Game::renderGame() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO: Possibly don't use pointers to bind in the future?
  cam.UploadToShader(&shader, window);
  shader.bind();
  va.bind();
  //  glDrawArrays(GL_TRIANGLES,0,va.elements);

  //  printf("%p - %p = %i\n",&ib,va.ib,&ib-va.ib);

  glDrawElements(GL_TRIANGLES, va.getElements(), GL_UNSIGNED_INT, 0);

  shader.unbind();
  va.unbind();

  glfwSwapBuffers(this->window);
  return 1;
}
