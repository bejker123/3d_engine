#ifndef __SHADER_H
#define __SHADER_H
#include "../logger.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory.h>

enum ShaderState { NOT_INITED = -1, ERROR = 0, INITED = 1 };

class Shader {
public:
  void init(const char *vertex_source, const char *fragment_source,
            const char *geometry_source);

  void bind();
  static void unbind();

  GLuint get_attrib_loc(char *attrib);

  void set1i(GLint value, const GLchar *name);
  void set1f(GLfloat value, const GLchar *name);
  void set_vec2f(glm::fvec2 value, const GLchar *name);
  void set_vec3f(glm::fvec3 value, const GLchar *name);
  void set_vec4f(glm::fvec4 value, const GLchar *name);

  void set_mat3fv(glm::mat3 value, const GLchar *name,
                  bool transpose = GL_FALSE);
  void set_mat4fv(glm::mat4 value, const GLchar *name,
                  bool transpose = GL_FALSE);

private:
  GLuint compile_shader(const char *source, const GLuint shader_type);
  GLuint id;
  int success;
};

#endif //__SHADER_H
