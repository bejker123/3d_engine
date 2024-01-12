#include "glfw.hpp"
#include "../../../io/logger.hpp"
namespace En {

std::optional<std::string> GLFW::get_error() {
  std::string desc;
  auto ret = glfwGetError((const char **)desc.data());

  if (ret != GL_NO_ERROR) { // Returns GL_NO_ERROR (0) if no error is present
    LOG("[OPENGL] GLFW ERROR: %d\n\t%s\n", ret, desc.data());
    return desc;
  }

  return std::nullopt;
}

void glfw_error_handler(int code, const char *desc) {
  LOG("[OPENGL] GLFW ERROR: %d\n\t%s\n", code, desc);
}

bool GLFW::setup() {
  if (!glfwInit()) {
    LOG("[OPENGL] GLFW INIT ERROR: \n\t%s\n", GLFW::get_error()->c_str());
    return false;
  }

  glfwSetErrorCallback(glfw_error_handler);

  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_LINE_SMOOTH);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  return true;
}
void GLFW::terminate() { glfwTerminate(); }
} // namespace En
