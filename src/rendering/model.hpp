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
  void setRot(glm::vec3 rot) { this->rot = rot; }

private:
  glm::mat4 mmatrix;
  std::shared_ptr<Material> mat;
  std::vector<std::shared_ptr<Mesh>> meshes;

  glm::vec3 pos, origin, rot, scale;
};
#endif // !MODEL_HPP
