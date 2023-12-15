#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <memory>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace En {
namespace ll {

class Texture {
public:
  Texture(){};
  Texture(const std::string file, const bool transparent = false,
          const GLenum type = GL_TEXTURE_2D);

  void terminate() const;

  GLuint get_id() const;

  void bind(const GLuint texture_unit) const;

  static void unbind(const uint32_t type = GL_TEXTURE_2D);

private:
  GLuint id = 0;
  int width, height;
  uint32_t type;
  bool transparent;

  void load_from_file(std::string file);
};

} // namespace ll
} // namespace En
using pTexture = std::shared_ptr<const En::ll::Texture>;
#endif // !TEXTURE_HPP
