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

private:
  glm::mat4 mmatrix;
  std::shared_ptr<Material> mat;
  std::shared_ptr<Mesh> mesh;
};
#endif // !MODEL_HPP
