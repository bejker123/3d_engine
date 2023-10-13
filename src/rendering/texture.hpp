#ifndef TEXTURE_HPP
#define TEXTURE_HPP
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class Texture {
public:
  Texture(const char *fileName, GLenum type = GL_TEXTURE_2D);

  ~Texture();

  inline GLuint getID() const;

  void bind(const GLuint texture_unit);

  void unbind();

private:
  GLuint id;
  int width, height;
  unsigned int type;

  void loadFromFile(char *fileName);
};

#endif // !TEXTURE_HPP
