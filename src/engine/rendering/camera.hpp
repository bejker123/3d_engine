#pragma once

#include "../io/window.hpp"
#include "ll/shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace En {

class Camera {
public:
  Camera();
  void init(float fov, float znear, float zfar, glm::vec3 pos);

  void upload_to_shader(pShader shader, Window *win);
  void upload_to_sb_shader(pShader shader, Window *win);

  void add_pos(glm::vec3 pos);

  void update_mosue_input(const float &dt, const double &offsetX,
                          const double &offsetY);

  const glm::vec3 get_front() const;
  const glm::vec3 get_right() const;
  const glm::vec3 get_up() const;
  const glm::vec3 get_world_up() const;
  glm::vec3 *get_pos();
  float *get_fov();
  float *get_yaw();
  float *get_pitch();
  float *get_roll();

private:
  void calculate_view_matrix();
  void calculate_proj_matrix(int fbw, int fbh);
  // void updateUniforms(Shader *shader, Window *win);

  void update_vectors();

  GLfloat sensitivity = 0.1;

  glm::vec3 pos;

  glm::mat4 view_matrix, proj_matrix;
  float fov, znear, zfar, aspect;

  glm::vec3 front;
  glm::vec3 right;
  glm::vec3 up, world_up;

  GLfloat pitch;
  GLfloat yaw;
  GLfloat roll;

  const GLfloat MAX_PITCH = 85.f;
  const GLfloat MAX_YAW = 360.f;
  const GLfloat MAX_ROLL = 0.f;
};
} // namespace En
