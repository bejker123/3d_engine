#include "opengl.hpp"
#include "../../io/logger.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>

namespace En {
namespace ll {

void opengl::clear_buffer() {
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

std::optional<std::string> opengl::get_error() {
  char *desc = (char *)malloc(1024);
  memset(desc, 0, 1024);
  auto ret = glfwGetError((const char **)&desc);

  if (ret != GL_NO_ERROR) { // Returns GL_NO_ERROR (0) if no error is present
    LOG("GLFW ERROR: %d\n\t%s\n", ret, desc);
    return desc;
  }

  free(desc);
  return std::nullopt;
}

void gl_debug_handler(GLenum source, GLenum type, GLuint id, GLenum severity,
                      GLsizei length, const GLchar *message,
                      const void *userParam) {
#if OPENGL_DEBUG_INFO
  std::string _source;
  std::string _type;
  std::string _severity;

  switch (source) {
  case GL_DEBUG_SOURCE_API:
    _source = "API";
    break;

  case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
    _source = "WINDOW SYSTEM";
    break;

  case GL_DEBUG_SOURCE_SHADER_COMPILER:
    _source = "SHADER COMPILER";
    break;

  case GL_DEBUG_SOURCE_THIRD_PARTY:
    _source = "THIRD PARTY";
    break;

  case GL_DEBUG_SOURCE_APPLICATION:
    _source = "APPLICATION";
    break;

  case GL_DEBUG_SOURCE_OTHER:
    _source = "UNKNOWN";
    break;

  default:
    _source = "UNKNOWN";
    break;
  }

  switch (type) {
  case GL_DEBUG_TYPE_ERROR:
    _type = "ERROR";
    break;

  case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
    _type = "DEPRECATED BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
    _type = "UDEFINED BEHAVIOR";
    break;

  case GL_DEBUG_TYPE_PORTABILITY:
    _type = "PORTABILITY";
    break;

  case GL_DEBUG_TYPE_PERFORMANCE:
    _type = "PERFORMANCE";
    break;

  case GL_DEBUG_TYPE_OTHER:
    _type = "OTHER";
    break;

  case GL_DEBUG_TYPE_MARKER:
    _type = "MARKER";
    break;

  default:
    _type = "UNKNOWN";
    break;
  }

  switch (severity) {
  case GL_DEBUG_SEVERITY_HIGH:
    _severity = "HIGH";
    break;

  case GL_DEBUG_SEVERITY_MEDIUM:
    _severity = "MEDIUM";
    break;

  case GL_DEBUG_SEVERITY_LOW:
    _severity = "LOW";
    break;

  case GL_DEBUG_SEVERITY_NOTIFICATION:
    _severity = "NOTIFICATION";
    break;

  default:
    _severity = "UNKNOWN";
    break;
  }

  LOG("[GL_DEBUG_MESSAGE] %s::%s::%s::ID: %d:\n\t%s\n", _source.c_str(),
      _type.c_str(), _severity.c_str(), id, message);
#endif
}

void glfw_error_handler(int code, const char *desc) {
  LOG("GLFW ERROR: %d\n\t%s\n", code, desc);
}

bool opengl::setup() {
  if (!glfwInit()) {
    LOG("GLFW INIT ERROR: \n\t%s\n", opengl::get_error()->c_str());
    return false;
  }

  glfwSetErrorCallback(glfw_error_handler);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJ);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MIN);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_LINE_SMOOTH);
  // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  return true;
}
bool opengl::setup_glew() { return glewInit() == GLEW_OK; }
void opengl::terminate() { glfwTerminate(); }
void opengl::debug_info() {

  LOG("OPENGL DEBUG INFO:\n");
  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  LOG("\tGraphics card vendor: %s\n", vendor);
  LOG("\tGraphics card: %s\n", renderer);
  LOG("\tOpenGL version: %s\n", version);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_debug_handler, nullptr);
}

} // namespace ll
} // namespace En
