#include "shader.hpp"
#include <assert.h>

void logEnd(int success, const char *file, int line) {
  LOG("[SHADER] INITING ENDED, CODE: %d, file: %s: %d\n", success, file, line);
}

#define LOG_END()                                                              \
  {                                                                            \
    logEnd(shader->success, __FILE__, __LINE__);                               \
    return;                                                                    \
  }

// TODO: Add a game id to every shader.
void initShader(Shader *shader, const char *vertex_source,
                const char *fragment_source, const char *geometry_source) {

  LOG("[SHADER] INITING STARTED\n");
  LOG("[SHADER] COMPILATION STARTED\n");

  // if((shader->success == 1)||(shader->success == 0))
  //     LOG_END();

  shader->success = -1;
  shader->id = -1;

  GLuint vertex = 0, fragment = 0, geometry = 0;

  if (!(vertex = compileShader(vertex_source, GL_VERTEX_SHADER))) {
    shader->success = 0;
    LOG_END();
  }

  if (!(fragment = compileShader(fragment_source, GL_FRAGMENT_SHADER))) {
    shader->success = 0;
    LOG_END();
  }

  if (strlen(geometry_source)) {
    if (!(geometry = compileShader(geometry_source, GL_GEOMETRY_SHADER))) {
      shader->success = 0;
      LOG_END();
    }
  }

  if (shader->success == 0) {
    LOG_END();
  } else
    LOG("[SHADER] COMPILATION SUCCESSFUL\n");

  LOG("[SHADER] LINKING STARTED\n");

  shader->id = glCreateProgram();

  glAttachShader(shader->id, vertex);
  glAttachShader(shader->id, fragment);

  if (strlen(geometry_source))
    glAttachShader(shader->id, geometry);

  glLinkProgram(shader->id);

  GLint success;

  char infoLog[512];
  memset(infoLog, 0, 512);

  glGetProgramiv(shader->id, GL_LINK_STATUS, &success);

  if (!success) {
    glGetProgramInfoLog(shader->id, 512, NULL, infoLog);
    LOG("[SHADER] Failed to link shader, %s\n", infoLog);
    shader->success = 0;
  } else {
    LOG("[SHADER] LINKING SUCCESSFUL\n");
    shader->success = 1;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
  if (strlen(geometry_source))
    glDeleteShader(geometry);

  LOG_END();
}

GLuint compileShader(const char *source, const GLuint shader_type) {

  GLuint id = glCreateShader(shader_type);

  glShaderSource(id, 1, &source, NULL);
  glCompileShader(id);

  int success;
  char infoLog[512];
  memset(infoLog, 0, 512);
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

    glGetShaderInfoLog(id, 512, NULL, infoLog);
    LOG("[SHADER] %s failed to compile, with error:\n\t %s", shader_type_name,
        infoLog);
    glDeleteShader(id);
    id = 0;
  }
  return id;
}

void bindShader(Shader *shader) {
  assert(shader->success == 1); // Check if shader is initialised
  glUseProgram(shader->id);
}

void unbindShader() { glUseProgram(0); }
