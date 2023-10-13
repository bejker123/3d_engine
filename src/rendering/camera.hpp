#include "../window.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
  void init(float fov, float znear, float zfar, glm::vec3 pos);

  void UploadToShader(Shader *shader, Window *win);

private:
  void CalculateViewMatrix();
  void CalculateProjMatrix(int fbw, int fbh);
  // void updateUniforms(Shader *shader, Window *win);

  glm::vec3 pos;

  glm::mat4 view_matrix, proj_matrix;
  float fov, znear, zfar, aspect;
};
