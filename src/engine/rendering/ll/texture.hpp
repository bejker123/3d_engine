#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;
namespace En {
namespace ll {

class Texture {
public:
  Texture(std::string file, GLenum type = GL_TEXTURE_2D);

  ~Texture();

  GLuint get_id() const;

  void bind(const GLuint texture_unit);

  static void unbind(const uint32_t type = GL_TEXTURE_2D);

private:
  GLuint id;
  int width, height;
  uint32_t type;

  void load_from_file(std::string file);
};

} // namespace ll
} // namespace En
#endif // !TEXTURE_HPP
