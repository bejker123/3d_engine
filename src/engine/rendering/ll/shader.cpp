#include "shader.hpp"
#include "../../io/logger.hpp"
#include <assert.h>

namespace En {

void log_end() { LOG("[SHADER] INITING ENDED\n"); }

Shader::Shader(const char *vertex_source, const char *fragment_source,
               const char *geometry_source) {
  this->init(vertex_source, fragment_source, geometry_source);
}

// TODO: Add a game id to every shader.
void Shader::init(const char *vertex_source, const char *fragment_source,
                  const char *geometry_source) {

  LOG("[SHADER] INITING STARTED\n");
  LOG("[SHADER] COMPILATION STARTED\n");

  this->success = -1;
  this->id = -1;

  GLuint vertex = 0, fragment = 0, geometry = 0;

  if (!(vertex = compile_shader(vertex_source, GL_VERTEX_SHADER))) {
    this->success = 0;
    log_end();
    return;
  }

  if (!(fragment = compile_shader(fragment_source, GL_FRAGMENT_SHADER))) {
    this->success = 0;
    log_end();
    return;
  }

  if (strlen(geometry_source)) {
    if (!(geometry = compile_shader(geometry_source, GL_GEOMETRY_SHADER))) {
      this->success = 0;
      log_end();
      return;
    }
  }

  if (this->success == 0) {
    log_end();
    return;
  } else
    LOG("[SHADER] COMPILATION SUCCESSFUL\n");

  LOG("[SHADER] LINKING STARTED\n");

  this->id = glCreateProgram();

  glAttachShader(this->id, vertex);
  glAttachShader(this->id, fragment);

  if (strlen(geometry_source))
    glAttachShader(this->id, geometry);

  glLinkProgram(this->id);

  GLint success;

  char info_log[512];
  memset(info_log, 0, 512);

  glGetProgramiv(this->id, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(this->id, 512, NULL, info_log);
    LOG("[SHADER] Failed to link shader, %s\n", info_log);
    this->success = 0;
  } else {
    LOG("[SHADER] LINKING SUCCESSFUL\n");
    this->success = 1;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (strlen(geometry_source))
    glDeleteShader(geometry);

  log_end();
}

void Shader::terminate() { glDeleteShader(this->id); }

GLuint Shader::compile_shader(const char *source, const GLuint shader_type) {

  GLuint id = glCreateShader(shader_type);

  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  int success;
  char info_log[512];
  memset(info_log, 0, 512);
  glGetShaderiv(id, GL_COMPILE_STATUS, &success);
  if (!success) {
    char shader_type_name[32];
    memset(shader_type_name, 0, 32);

    strcpy(shader_type_name, "UNKNOWN");

    if (shader_type == GL_VERTEX_SHADER)
      strcpy(shader_type_name, "GL_VERTEX_SHADER");
    else if (shader_type == GL_FRAGMENT_SHADER)
      strcpy(shader_type_name, "GL_FRAGMENT_SHADER");
    else if (shader_type == GL_GEOMETRY_SHADER)
      strcpy(shader_type_name, "GL_GEOMETRY_SHADER");

    glGetShaderInfoLog(id, 512, NULL, info_log);
    LOG("[SHADER] %s failed to compile, with error:\n\t %s\n", shader_type_name,
        info_log);
    glDeleteShader(id);
    id = 0;
  }
  return id;
}

void Shader::bind() const {
  assert(this->success == 1); // Check if shader is initialised
  glUseProgram(this->id);
}

void Shader::unbind() { glUseProgram(0); };
GLuint Shader::get_attrib_loc(char *attrib) const {
  return glGetAttribLocation(this->id, attrib);
}

void Shader::set1i(GLint value, const GLchar *name) const {
  this->bind();
  glUniform1i(glGetUniformLocation(this->id, name), value);
  Shader::unbind();
}

void Shader::set1f(GLfloat value, const GLchar *name) const {
  this->bind();
  glUniform1f(glGetUniformLocation(this->id, name), value);
  Shader::unbind();
}

void Shader::set_vec2f(glm::fvec2 value, const GLchar *name) const {
  this->bind();
  glUniform2fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
  Shader::unbind();
}

void Shader::set_vec3f(glm::fvec3 value, const GLchar *name) const {
  this->bind();
  glUniform3fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
  Shader::unbind();
}

void Shader::set_vec4f(glm::fvec4 value, const GLchar *name) const {
  this->bind();
  glUniform4fv(glGetUniformLocation(this->id, name), 1, glm::value_ptr(value));
  Shader::unbind();
}

void Shader::set_mat3fv(glm::mat3 value, const GLchar *name,
                        bool transpose) const {
  this->bind();
  glUniformMatrix3fv(glGetUniformLocation(this->id, name), 1, transpose,
                     glm::value_ptr(value));
  Shader::unbind();
}

void Shader::set_mat4fv(glm::mat4 value, const GLchar *name,
                        bool transpose) const {
  this->bind();
  glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, transpose,
                     glm::value_ptr(value));
  Shader::unbind();
}

} // namespace En
