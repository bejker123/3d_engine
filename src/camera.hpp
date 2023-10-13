#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
  void init(float fov, float znear, float zfar, glm::vec3 pos);

  void UploadToShader(Shader *shader, GLFWwindow *window);

private:
  void CalculateViewMatrix();
  void CalculateProjMatrix(GLFWwindow *window);
  void updateUniforms(Shader *shader, GLFWwindow *window);

  glm::vec3 pos;

  glm::mat4 view_matrix, proj_matrix;
  float fov, znear, zfar, aspect;
};
