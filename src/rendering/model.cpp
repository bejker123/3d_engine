#include "model.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

void Model::init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) {
  this->mesh = mesh;
  this->mat = mat;

  this->origin = glm::vec3(0);
  this->pos = glm::vec3(0);
  this->rot = glm::vec3(0, 0, 0);
  this->scale = glm::vec3(10);
}

void Model::update() {
  this->mmatrix = glm::mat4(1.f);
  this->mmatrix = glm::translate(this->mmatrix, this->origin);
  this->mmatrix = glm::rotate(this->mmatrix, glm::radians(this->rot.x),
                              glm::vec3(1.f, 0.f, 0.f));
  this->mmatrix = glm::rotate(this->mmatrix, glm::radians(this->rot.y),
                              glm::vec3(0.f, 1.f, 0.f));
  this->mmatrix = glm::rotate(this->mmatrix, glm::radians(this->rot.z),
                              glm::vec3(0.f, 0.f, 1.f));
  this->mmatrix = glm::translate(this->mmatrix, this->pos - this->origin);
  this->mmatrix = glm::scale(this->mmatrix, this->scale);
  this->mat->getShader()->setMat4fv(this->mmatrix, "ModelMatrix");
}

void Model::render() {
  this->update();
  this->mat->bind();
  this->mesh->render();
  this->mat->unbind();
}
