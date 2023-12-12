#ifndef WINDOW_HPP
#define WINDOW_HPP
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

  void set_should_close(bool val);

  void swap_buffers() const;

  GLFWwindow *raw() const;

  int get_key(int key) const;

  std::tuple<int, int> get_dimensions();

  std::tuple<double, double> get_mouse_pos();

  void hide_cursor() const;

  void show_cursor() const;

private:
  GLFWwindow *window;
};

} // namespace En
#endif // !WINDOW_HPP
