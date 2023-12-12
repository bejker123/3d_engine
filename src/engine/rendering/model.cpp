#include "model.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <string>

void En::Model::init(std::shared_ptr<Mesh> mesh,
                     std::shared_ptr<Material> mat) {
  this->meshes.push_back(mesh);
  this->mat = mat;

  this->origin = glm::vec3(0);
  this->pos = glm::vec3(0);
  this->rot = glm::vec3(0, 0, 0);
  this->scale = glm::vec3(10);
}

void En::Model::update() {
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
  this->mat->get_shader()->set_mat4fv(this->mmatrix, "ModelMatrix");
}

void En::Model::render() {
  this->update();
  this->mat->bind();
  for (auto &i : this->meshes) {
    i->render();
  }
  this->mat->unbind();
}

void En::Model::load(std::string path) {
  Assimp::Importer import;
  const aiScene *scene =
      import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
    // return;
  }
}

void processNode(aiNode *node, const aiScene *scene) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    // meshes.push_back(processMesh(mesh, scene));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    processNode(node->mChildren[i], scene);
  }
}

En::Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
  std::vector<En::ll::Vertex> vertices;
  std::vector<unsigned int> indices;
  // std::vector<Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    // Vertex vertex;
    // process vertex positions, normals and texture coordinates
    // vertices.push_back(vertex);
  }
  // process indices
  // process material
  if (mesh->mMaterialIndex >= 0) {
  }

  return En::Mesh(vertices, indices);
}

void En::Model::set_pos(glm::vec3 pos) { this->pos = pos; }
void En::Model::set_origin(glm::vec3 origin) {
  this->origin = origin;
  this->pos = origin;
}
void En::Model::set_rot(glm::vec3 rot) { this->rot = rot; }
glm::vec3 *En::Model::get_rot() { return &this->rot; }
glm::vec3 *En::Model::get_pos() { return &this->pos; }
glm::vec3 *En::Model::get_origin() { return &this->origin; }
glm::vec3 *En::Model::get_scale() { return &this->scale; }
std::shared_ptr<En::Material> En::Model::get_material() { return this->mat; }
