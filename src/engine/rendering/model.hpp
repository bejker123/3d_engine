#ifndef MODEL_HPP
#define MODEL_HPP
#include "mesh.hpp"

namespace En {

class Model {
public:
  Model();
  Model(pMesh mesh);
  Model(std::vector<pMesh> meshes);
  void init(std::shared_ptr<Mesh> mesh);

  void update();
  void render();
  void render_batch();

  void set_pos(glm::vec3 pos);
  void set_scale(glm::vec3 scale);
  void set_origin(glm::vec3 origin);
  void set_rot(glm::vec3 rot);

  std::vector<pMesh> get_meshes();

  glm::vec3 *get_rot();
  glm::vec3 *get_pos();
  glm::vec3 *get_origin();
  glm::vec3 *get_scale();

private:
  glm::mat4 mmatrix;
  std::vector<pMesh> meshes;

  glm::vec3 pos, origin, rot, scale;
};
} // namespace En
#endif // !MODEL_HPP
