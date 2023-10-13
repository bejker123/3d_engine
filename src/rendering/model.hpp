#ifndef MODEL_HPP
#define MODEL_HPP
#include "material.hpp"
#include "mesh.hpp"
#include "shader.hpp"
#include <memory>

class Model {
public:
  void init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);

  void update();
  void render();
  void render_batch();

  void load(std::string path);

  void setPos(glm::vec3 pos) { this->pos = pos; }
  void setOrigin(glm::vec3 origin) {
    this->origin = origin;
    this->pos = origin;
  }
  void setRot(glm::vec3 rot) { this->rot = rot; }
  glm::vec3 *getRot() { return &this->rot; }
  glm::vec3 *getPos() { return &this->pos; }
  glm::vec3 *getOrigin() { return &this->origin; }
  glm::vec3 *getScale() { return &this->scale; }
  std::shared_ptr<Material> getMaterial() { return this->mat; }

private:
  glm::mat4 mmatrix;
  std::shared_ptr<Material> mat;
  std::vector<std::shared_ptr<Mesh>> meshes;

  glm::vec3 pos, origin, rot, scale;
};
#endif // !MODEL_HPP
