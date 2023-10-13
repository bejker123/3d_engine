#include "model.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

void Model::init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) {
  this->mesh = mesh;
  this->mat = mat;
  this->mmatrix = glm::mat4(1);
  this->mmatrix = glm::scale(this->mmatrix, glm::vec3(10.0));
}

void Model::update() {
  this->mat->getShader()->setMat4fv(this->mmatrix, "ModelMatrix");
}

void Model::render() {
  this->mat->bind();
  this->mesh->render();
  this->mat->unbind();
}
