#include "game.hpp"
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

_GS GS;

float vertices[] = {
    0.5f,  0.5f,  0.0f, // top right
    0.5f,  -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f,  0.0f  // top left
};
unsigned int indices[] = {
    // note that we start from 0!
    0, 1, 3, // first Triangle
    1, 2, 3  // second Triangle
};

float colors[] = {
    0.3f, 1.f, 0.5f, 1.f, 0.3f, 1.f, 0.5f, 1.f,
    0.3f, 1.f, 0.5f, 1.f, 0.3f, 1.f, 0.5f, 1.f,
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

// Init Game State, run the main loop
int initGS(int argc, char *argv[]) {
  LOG("INITIALISATION STARTED\n");

  // First set the game state to uninitialised
  GS.inited = false;
  GS.window = NULL;
  GS.monitor = NULL;

  // Run initialisation functions
  initCommandLineArgs(argc, argv);

  if (!initOpenGL())
    return EXIT_FAILURE;

  LOG_VAR("%d", shader.id);
  LOG_VAR("%d", shader.success);

  initShader(&shader, basic_vertex_shader, basic_fragment_shader, "");

  LOG_VAR("%d", shader.id);
  LOG_VAR("%d", shader.success);

  va = initVertexArray();

  initVertexBuffer(&vb, vertices, sizeof(vertices), 3 * sizeof(float));

  initVertexBuffer(&vb1, colors, sizeof(colors), 4 * sizeof(float));

  initIndexBuffer(&ib, indices, sizeof(indices));

  addVertexBuffer(&va, &vb);
  addVertexBuffer(&va, &vb1);
  setIndexBuffer(&va, &ib);

  LOG_VAR("%d", vb.id);
  LOG_VAR("%d", vb.count);
  LOG_VAR("%d", ib.elements);
  LOG_VAR("%d", ib.id);
  LOG_VAR("%d", va.id);
  // LOG_VAR("%p",va.ib);

  // If all initialisation functions run succesfully
  // set game state to inited
  GS.inited = true;

  // Run the main loop
  return runGame();
}

// Parse and init cmd line args
void initCommandLineArgs(int argc, char *argv[]) {
  LOG("INITIALISING Command Line Arguments\n");
  GS.options.window_title = (char *)"test";
  GS.options.window_width = 800;
  GS.options.window_height = 600;
  GS.options.window_resizable = false;
  GS.options.window_fullscreen = false;
  GS.monitor = NULL;
}

// Initialise OpenGL, return true if successful
bool initOpenGL() {
  LOG("INITIALISING OPENGL\n");
  LOG("INITIALISING GLFW\n");
  if (!glfwInit()) {
    LOG("FAILED TO INIT GLFW\n");
    terminateOpenGL();
    return false;
  }

  LOG("INITIALISING WINDOW\n");

  printf("%p\n", GS.monitor);
  if (GS.options.window_fullscreen) {
    GS.monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(GS.monitor, 0, 0, &GS.options.window_width,
                           &GS.options.window_height);
  }

  // set opengl version to 3.3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJ);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MIN);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GS.options.window_resizable);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  GS.window =
      glfwCreateWindow(GS.options.window_width, GS.options.window_height,
                       GS.options.window_title, GS.monitor, NULL);

  if (GS.window == NULL) {
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

 glfwGetFramebufferSize(GS.window, &frameBufferWidth, &frameBufferHeight);
        glViewport(0, 0, frameBufferWidth, frameBufferHeight);

        glfwSetFramebufferSizeCallback(GS.window,
 (GLFWframebuffersizefun)framebuffer_resize_callback);
*/

  LOG("INITIALISING GLEW\n");

  glfwMakeContextCurrent(GS.window);

  glewExperimental = GL_TRUE;

  if (glfwGetCurrentContext() != GS.window) {
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

void terminateGS() {
  LOG("TERMINATION STARTED\n");

  terminateOpenGL();

  LOG("TERMINATION COMPELTE\n");
}

void terminateOpenGL() {
  LOG("TERMINATING OPENGL\n");
  glfwDestroyWindow(GS.window);
  glfwTerminate();
}

// Main game loop
int runGame() {
  while (!glfwWindowShouldClose(GS.window)) {
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
int updateGame() {
  glfwPollEvents();

  if (!handleKeyboard())
    return 0;
  // PRINT_VAR(x);
  return 1;
}

// Handles keyboard user input
// runs every frame
int handleKeyboard() {
  if (glfwGetKey(GS.window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(GS.window, true);

  return 1;
}

// Rendering function running every frame, after update
int renderGame() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT);

  // TODO: Possibly don't use pointers to bind in the future?
  bindShader(&shader);
  bindVertexArray(&va);

  //  glDrawArrays(GL_TRIANGLES,0,va.elements);

  //  printf("%p - %p = %i\n",&ib,va.ib,&ib-va.ib);

  glDrawElements(GL_TRIANGLES, va.elements, GL_UNSIGNED_INT, 0);

  unbindShader();
  unbindVertexArray();

  glfwSwapBuffers(GS.window);
  return 1;
}
