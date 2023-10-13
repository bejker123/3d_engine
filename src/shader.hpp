#ifndef __SHADER_H
#define __SHADER_H
#include "logger.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
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

  GLuint getAttribLocation(char *attrib);

  void set1i(GLint value, const GLchar *name);
  void set1f(GLfloat value, const GLchar *name);
  void setVec2f(glm::fvec2 value, const GLchar *name);
  void setVec3f(glm::fvec3 value, const GLchar *name);
  void setVec4f(glm::fvec4 value, const GLchar *name);

  void setMat3fv(glm::mat3 value, const GLchar *name,
                 bool transpose = GL_FALSE);
  void setMat4fv(glm::mat4 value, const GLchar *name,
                 bool transpose = GL_FALSE);

private:
  GLuint compileShader(const char *source, const GLuint shader_type);
  GLuint id;
  int success;
};

#endif //__SHADER_H
