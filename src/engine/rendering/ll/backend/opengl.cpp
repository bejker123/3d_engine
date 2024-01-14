#include "opengl.hpp"
#include <GLFW/glfw3.h>

#define LOGGER_PREFIX "[OPENGL] "
#include "../../../io/logger.hpp"

namespace En {

void opengl::clear_buffer() {
  glDepthMask(GL_TRUE);
  glStencilMask(GL_TRUE);
  glClearColor(1.f, 1.f, 1.f, 1.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
  glDepthMask(GL_FALSE);
  glStencilMask(GL_FALSE);
}
bool opengl::setup_glew() {
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VER_MAJ);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VER_MIN);
  glewExperimental = GL_TRUE;
  return glewInit() == GLEW_OK;
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

  if (severity == GL_DEBUG_SEVERITY_HIGH) {
    LOG("[OPENGL] [GL_DEBUG_MESSAGE] %s::%s::%s::%d:\n\t%s\n", _source.c_str(),
        _type.c_str(), _severity.c_str(), id, message);
  }
#endif
}
void opengl::debug_info() {

  const GLubyte *vendor = glGetString(GL_VENDOR);
  const GLubyte *renderer = glGetString(GL_RENDERER);
  const GLubyte *version = glGetString(GL_VERSION);
  LOG("OPENGL DEBUG INFO:\n\tGraphics Card Vendor: %s\n\tGraphics "
      "Card: %s\n\tOpenGL Version: %s\n",
      vendor, renderer, version);
  glEnable(GL_DEBUG_OUTPUT);
  glDebugMessageCallback(gl_debug_handler, nullptr);
}

} // namespace En
