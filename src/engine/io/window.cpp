#include "window.hpp"
#include "logger.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>

namespace En {

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

bool Window::init(int width, int height, char *title, bool resizable,
                  bool fullscreen, bool vsync) {
  glfwWindowHint(GLFW_RESIZABLE, resizable);
  GLFWmonitor *monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, 0, 0, &width, &height);
  }
  this->window = glfwCreateWindow(width, height, title, monitor, NULL);

  if (this->window == NULL) {
    LOG("FAILED TO INIT WINDOW\n");
    const char *x = (const char *)malloc(1025);
    memset((void *)x, '0', 1025);
    glfwGetError(&x);
    LOG(x);
    printf("\n");
    return false;
  }

  glfwMakeContextCurrent(this->window);
  if (glfwGetCurrentContext() != this->window) {
    LOG("FAIELD TO INIT WINDOW\n");
    return false;
  }

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  if (!vsync) // Disable V-Sync
    glfwSwapInterval(0);

  return true;
}

void Window::terminate() {
  glfwSetWindowShouldClose(this->window, GL_TRUE);
  glfwDestroyWindow(this->window);
}

bool Window::should_close() const {
  return glfwWindowShouldClose(this->window);
}
void Window::set_should_close(bool val) const {
  glfwSetWindowShouldClose(this->window, val);
}

void Window::swap_buffers() const { glfwSwapBuffers(this->window); };

GLFWwindow *Window::raw() const { return this->window; };

std::tuple<int, int> Window::get_dimensions() const {
  int width, height;

  glfwGetFramebufferSize(window, &width, &height);
  return std::make_tuple(width, height);
}

std::tuple<double, double> Window::get_mouse_pos() const {
  double x, y;
  glfwGetCursorPos(this->window, &x, &y);
  return std::make_tuple(x, y);
}

void Window::hide_cursor() const {
  glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::show_cursor() const {
  glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

} // namespace En
