#include "window.hpp"
#include "logger.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>

namespace En {

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
  if (!vsync) // Disable V-Sync
    glfwSwapInterval(0);

  return true;
}

void Window::terminate() { glfwDestroyWindow(this->window); }

bool Window::should_close() const {
  return glfwWindowShouldClose(this->window);
}
void Window::set_should_close(bool val) {
  glfwSetWindowShouldClose(this->window, val);
}

void Window::swap_buffers() const { glfwSwapBuffers(this->window); };

GLFWwindow *Window::raw() const { return this->window; };

int Window::get_key(int key) const { return glfwGetKey(this->window, key); }

std::tuple<int, int> Window::get_dimensions() {
  int frameBufferWidth, frameBufferHeight;

  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
  return std::make_tuple(frameBufferWidth, frameBufferHeight);
}

std::tuple<double, double> Window::get_mouse_pos() {
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
