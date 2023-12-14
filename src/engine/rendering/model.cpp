#include "model.hpp"
#include "ll/texture.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include <memory>
#include <string>

namespace En {
Model::Model() {
  this->origin = glm::vec3(0);
  this->pos = glm::vec3(0);
  this->rot = glm::vec3(0, 0, 0);
  this->scale = glm::vec3(10);
}
Model::Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) {
  this->init(mesh, mat);
}

void Model::init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat) {
  this->meshes.push_back(mesh);
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
  this->mat->get_shader()->set_mat4fv(this->mmatrix, "ModelMatrix");
}

void Model::render() {
  this->update();
  this->mat->bind();
  for (auto &i : this->meshes) {
    i->render();
  }
  this->mat->unbind();
}

ll::Texture *tex0;
bool tex0_loaded = false;

void load_texture(aiMaterial *mat, aiTextureType type, std::string directory) {
  for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
    aiString str;
    mat->GetTexture(type, i, &str);
    // std::cout << directory << std::endl;
    tex0 = new ll::Texture(directory + str.C_Str());
    tex0_loaded = true;
    return;
  }
}

Mesh process_mesh(aiMesh *mesh, const aiScene *scene, const std::string &dir) {
  std::vector<ll::VertexC> vertices;
  std::vector<unsigned int> indices;
  // std::vector<ll::Texture> textures;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    auto vert = mesh->mVertices[i];
    auto norm = mesh->mNormals[i];

    auto _text = mesh->mTextureCoords[0];
    auto text = aiVector3D(0, 0, 0);

    if (_text)
      text = _text[i];

    vertices.push_back(ll::VertexC(
        glm::vec3(vert.x, vert.y, vert.z), glm::vec3(norm.x, norm.y, norm.z),
        glm::vec2(text.x, text.y), glm::vec4(0.2, 0.2, 0.2, 1)));

    // Vertex vertex;
    // process vertex positions, normals and texture coordinates
    // vertices.push_back(vertex);
  }

  // process indices
  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  // process material
  if (mesh->mMaterialIndex >= 0 && !tex0_loaded) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    load_texture(material, aiTextureType_DIFFUSE, dir);
  }

  return Mesh(vertices, indices);
}

void Model::process_node(aiNode *node, const aiScene *scene,
                         const std::string &dir) {
  // process all the node's meshes (if any)
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(std::make_shared<Mesh>(process_mesh(mesh, scene, dir)));
  }
  // then do the same for each of its children
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene, dir);
  }
}

std::optional<ll::Texture *> Model::load(std::string path,
                                         std::shared_ptr<Material> mat) {
  this->mat = mat;
  this->origin = glm::vec3(0);
  this->pos = glm::vec3(0);
  this->rot = glm::vec3(0, 0, 0);
  this->scale = glm::vec3(10);
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    // return;
    return std::nullopt;
  }
  std::string dir = path.substr(0, path.find_last_of('/')) + "/";
  process_node(scene->mRootNode, scene, dir);
  return std::optional(tex0);
}

void Model::set_pos(glm::vec3 pos) { this->pos = pos; }
void Model::set_origin(glm::vec3 origin) {
  this->origin = origin;
  this->pos = origin;
}
void Model::set_rot(glm::vec3 rot) { this->rot = rot; }
glm::vec3 *Model::get_rot() { return &this->rot; }
glm::vec3 *Model::get_pos() { return &this->pos; }
glm::vec3 *Model::get_origin() { return &this->origin; }
glm::vec3 *Model::get_scale() { return &this->scale; }
std::shared_ptr<Material> Model::get_material() { return this->mat; }

} // namespace En
