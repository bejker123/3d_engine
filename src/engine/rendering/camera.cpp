#include "camera.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace En {

Camera::Camera() {
  this->world_up = glm::vec3(0, 1, 0);
  this->right = glm::vec3(0.f);
  this->up = world_up;

  this->pitch = -20.f;
  this->yaw = -30.f;
  this->roll = 0.f;

  this->update_vectors();
}

void Camera::init(float fov, float znear, float zfar, glm::vec3 pos) {
  this->fov = fov;
  this->znear = znear;
  this->zfar = zfar;
  this->pos = pos;
}

void Camera::calculate_proj_matrix(int fbw, int fbh) {

  this->proj_matrix = glm::mat4(1.f);
  this->proj_matrix =
      glm::perspective(glm::radians(this->fov), static_cast<float>(fbw) / fbh,
                       this->znear, this->zfar);
}

void Camera::calculate_view_matrix() {
  // this->updateCameraVectors();
  this->view_matrix = glm::lookAt(this->pos, this->pos + this->front, this->up);
}

void Camera::upload_to_shader(pShader shader, Window *win) {

  this->update_vectors();

  this->calculate_view_matrix();

  // auto [fbw, fbh] = win->get_dimensions();

  // FIXED SIZE
  this->calculate_proj_matrix(1920, 1080);

  shader->set_mat4fv(this->view_matrix, "ViewMatrix");
  shader->set_mat4fv(this->proj_matrix, "ProjectionMatrix");
  shader->set_vec3f(this->pos, "cameraPos");
}
void Camera::add_pos(glm::vec3 pos) { this->pos += pos; }

void Camera::update_mosue_input(const float &dt, const double &offsetX,
                                const double &offsetY) {
  if (offsetX == 0 && offsetY == 0)
    return;
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

const glm::vec3 Camera::get_front() const { return this->front; }
const glm::vec3 Camera::get_right() const { return this->right; }
const glm::vec3 Camera::get_up() const { return this->up; }
const glm::vec3 Camera::get_world_up() const { return this->world_up; }
glm::vec3 *Camera::get_pos() { return &this->pos; };
float *Camera::get_fov() { return &this->fov; };
float *Camera::get_pitch() { return &this->pitch; };
float *Camera::get_yaw() { return &this->yaw; };
float *Camera::get_roll() { return &this->roll; };

void Camera::update_vectors() {
  this->front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
  this->front.y = sin(glm::radians(this->pitch));
  this->front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));

  this->front = glm::normalize(this->front);
  this->right = glm::normalize(glm::cross(this->front, this->world_up));
  this->up =
      glm::normalize(glm::cross(this->right, this->front)); // = this->worldUp
}

} // namespace En
