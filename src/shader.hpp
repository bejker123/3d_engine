#ifndef __SHADER_H
#define __SHADER_H
#include "logger.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory.h>
#include <stdbool.h>
#include <stdlib.h>

enum ShaderState { NOT_INITED = -1, ERROR = 0, INITED = 1 };

class Shader {
public:
  void init(const char *vertex_source, const char *fragment_source,
            const char *geometry_source);

  void bind();
  void unbind();

private:
  GLuint compileShader(const char *source, const GLuint shader_type);
  GLuint id;
  int success;
};

#endif //__SHADER_H
