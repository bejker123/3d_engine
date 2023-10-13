#include "window.hpp"
#include "logger.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstring>

bool Window::init(int width, int height, char *title, bool resizable,
                  bool fullscreen) {
  glfwWindowHint(GLFW_RESIZABLE, resizable);
  GLFWmonitor *monitor = nullptr;
  if (fullscreen) {
    monitor = glfwGetPrimaryMonitor();
    glfwGetMonitorWorkarea(monitor, 0, 0, &width, &height);
  }
  this->window = glfwCreateWindow(width, height, title, monitor, NULL);

  if (this->window == NULL) {
    LOG("FAILED TO INIT WINDOW\n");
    const char *x = (const char *)malloc(1024);
    memset((void *)x, '0', 1024);
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

  return true;
}
