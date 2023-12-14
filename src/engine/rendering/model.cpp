#include "model.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <memory>

namespace En {
void Model::set_values() {
  this->origin = glm::vec3(0);
  this->pos = glm::vec3(0);
  this->rot = glm::vec3(0, 0, 0);
  this->scale = glm::vec3(10);
}

Model::Model() { this->set_values(); }
Model::Model(pMesh mesh) { this->init(mesh); }
Model::Model(std::vector<pMesh> meshes) {
  this->set_values();
  this->meshes = meshes;
}

void Model::init(std::shared_ptr<Mesh> mesh) {
  this->set_values();
  this->meshes.push_back(mesh);
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

  for (auto &i : this->meshes)
    i->get_material()->get_shader()->set_mat4fv(this->mmatrix, "ModelMatrix");
}

void Model::render() {
  this->update();
  for (auto &i : this->meshes) {
    i->render();
  }
}

void Model::set_pos(glm::vec3 pos) { this->pos = pos; }
void Model::set_scale(glm::vec3 scale) { this->scale = scale; }
void Model::set_origin(glm::vec3 origin) {
  this->origin = origin;
  this->pos = origin;
}
void Model::set_rot(glm::vec3 rot) { this->rot = rot; }

std::vector<pMesh> Model::get_meshes() { return this->meshes; }
glm::vec3 *Model::get_rot() { return &this->rot; }
glm::vec3 *Model::get_pos() { return &this->pos; }
glm::vec3 *Model::get_origin() { return &this->origin; }
glm::vec3 *Model::get_scale() { return &this->scale; }

} // namespace En
