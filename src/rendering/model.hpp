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

  void set_pos(glm::vec3 pos);
  void set_origin(glm::vec3 origin);
  void set_rot(glm::vec3 rot);

  glm::vec3 *get_rot();
  glm::vec3 *get_pos();
  glm::vec3 *get_origin();
  glm::vec3 *get_scale();

  std::shared_ptr<Material> get_material();

private:
  glm::mat4 mmatrix;
  std::shared_ptr<Material> mat;
  std::vector<std::shared_ptr<Mesh>> meshes;

  glm::vec3 pos, origin, rot, scale;
};
#endif // !MODEL_HPP
