#ifndef MODEL_HPP
#define MODEL_HPP
#include "material.hpp"
#include "mesh.hpp"
#include <assimp/scene.h>

namespace En {

class Model {
public:
  Model();
  Model(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);
  void init(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> mat);

  void update();
  void render();
  void render_batch();

  // Returns true on success
  std::optional<ll::Texture> load(std::string path,
                                  std::shared_ptr<Material> mat);

  void set_pos(glm::vec3 pos);
  void set_origin(glm::vec3 origin);
  void set_rot(glm::vec3 rot);

  glm::vec3 *get_rot();
  glm::vec3 *get_pos();
  glm::vec3 *get_origin();
  glm::vec3 *get_scale();

  std::shared_ptr<Material> get_material();

private:
  void process_node(aiNode *node, const aiScene *scene, const std::string &dir);

  glm::mat4 mmatrix;
  std::shared_ptr<Material> mat;
  std::vector<std::shared_ptr<Mesh>> meshes;

  glm::vec3 pos, origin, rot, scale;
};
} // namespace En
#endif // !MODEL_HPP
