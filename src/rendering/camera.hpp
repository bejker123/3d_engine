#include "../window.hpp"
#include "shader.hpp"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Camera {
public:
  Camera() {

    this->world_up = glm::vec3(0, 1, 0);
    this->right = glm::vec3(0.f);
    this->up = world_up;

    this->pitch = 0.f;
    this->yaw = -90.f;
    this->roll = 0.f;

    this->updateVectors();
  }
  void init(float fov, float znear, float zfar, glm::vec3 pos);

  void UploadToShader(Shader *shader, Window *win);

  void addPos(glm::vec3 pos) { this->pos += pos; }

  void updateMosueInput(const float &dt, const double &offsetX,
                        const double &offsetY) {
    // Update pitch yaw and roll
    this->pitch += static_cast<float>(offsetY) * this->sensitivity * dt; // Y
    this->yaw += static_cast<float>(offsetX) * this->sensitivity * dt;   // X

    if (this->pitch > this->MAX_PITCH && this->pitch > 0.f)
      this->pitch = this->MAX_PITCH;
    else if (this->pitch < -this->MAX_PITCH && this->pitch < 0.f)
      this->pitch = -this->MAX_PITCH;

    if (this->yaw > this->MAX_YAW || this->yaw < -this->MAX_YAW)
      this->yaw = 0.f;

    // DEBUG
    // cout << "Yaw:" << this->yaw << " Pitch:" << this->pitch << endl;
  }

  glm::vec3 getFront() const { return this->front; }
  glm::vec3 getRight() const { return this->right; }
  glm::vec3 getUp() const { return this->up; }
  glm::vec3 getWorldUp() const { return this->world_up; }

private:
  void CalculateViewMatrix();
  void CalculateProjMatrix(int fbw, int fbh);
  // void updateUniforms(Shader *shader, Window *win);

  void updateVectors() {
    this->front.x =
        cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
    this->front.y = sin(glm::radians(this->pitch));
    this->front.z =
        sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

    this->front = glm::normalize(this->front);
    this->right = glm::normalize(glm::cross(this->front, this->world_up));
    this->up =
        glm::normalize(glm::cross(this->right, this->front)); // = this->worldUp
  }

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
