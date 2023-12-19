#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <tuple>
namespace En {
class Window {
public:
  bool init(int width, int height, char *title, bool resizable, bool fullscreen,
            bool vsync = false);

  void terminate();

  bool should_close() const;

  void set_should_close(bool val) const;

  void swap_buffers() const;

  GLFWwindow *raw() const;

  std::tuple<int, int> get_dimensions() const;

  std::tuple<double, double> get_mouse_pos() const;

  void hide_cursor() const;

  void show_cursor() const;

private:
  GLFWwindow *window;
};

} // namespace En
