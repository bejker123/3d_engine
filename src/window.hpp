#ifndef WINDOW_HPP
#define WINDOW_HPP
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
class Window {
public:
  bool init(int width, int height, char *title, bool fullscreen);

  void terminate() { glfwDestroyWindow(this->window); }
  bool shouldClose() const { return glfwWindowShouldClose(this->window); }
  void setShouldClose(bool val) { glfwSetWindowShouldClose(this->window, val); }
  void swapBuffers() const { glfwSwapBuffers(this->window); };
  GLFWwindow *raw() const { return this->window; };

  int getKey(int key) const { return glfwGetKey(this->window, key); }

  std::tuple<int, int> getDimensions() {
    int frameBufferWidth, frameBufferHeight;

    glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);
    return std::make_tuple(frameBufferWidth, frameBufferHeight);
  }

  std::tuple<double, double> getMousePos() {
    double x, y;
    glfwGetCursorPos(this->window, &x, &y);
    return std::make_tuple(x, y);
  }

  void hideCursor() const {
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  }
  void showCursor() const {
    glfwSetInputMode(this->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
  }

private:
  GLFWwindow *window;
};
#endif // !WINDOW_HPP
