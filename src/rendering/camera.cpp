#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

void Camera::init(float fov, float znear, float zfar, glm::vec3 pos) {
  this->fov = fov;
  this->znear = znear;
  this->zfar = zfar;
  this->pos = pos;
}

void Camera::CalculateProjMatrix(GLFWwindow *window) {
  int frameBufferWidth, frameBufferHeight;

  glfwGetFramebufferSize(window, &frameBufferWidth, &frameBufferHeight);

  this->proj_matrix = glm::mat4(1.f);
  this->proj_matrix =
      glm::perspective(glm::radians(this->fov),
                       static_cast<float>(frameBufferWidth) / frameBufferHeight,
                       this->znear, this->zfar);
}

void Camera::CalculateViewMatrix() {
  // this->updateCameraVectors();

  this->view_matrix = glm::lookAt(this->pos, this->pos + glm::vec3(1, 0, 0),
                                  glm::vec3(0, 1, 0));
}

void Camera::UploadToShader(Shader *shader, GLFWwindow *window) {

  this->CalculateViewMatrix();

  this->CalculateProjMatrix(window);

  shader->setMat4fv(this->view_matrix, "ViewMatrix");
  shader->setMat4fv(this->proj_matrix, "ProjectionMatrix");
  shader->setVec3f(this->pos, "cameraPos");
}
