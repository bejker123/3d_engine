#ifndef MESH_HPP
#define MESH_HPP
#include "ll/vertex_array.hpp"
#include "material.hpp"
#include <memory>
#include <vector>

namespace En {

class Mesh {
public:
  void init(pMaterial material, std::shared_ptr<ll::VertexArray> va);

  Mesh(pMaterial material, std::vector<ll::Vertex> vertices,
       std::vector<unsigned int> indices);
  Mesh(pMaterial material, std::vector<ll::VertexC> vertices,
       std::vector<unsigned int> indices);
  Mesh(){};
  void add_va(std::shared_ptr<ll::VertexArray> va);
  pMaterial get_material();

  void render();

private:
  std::vector<std::shared_ptr<ll::VertexArray>> vas;
  pMaterial mat;
};

} // namespace En
using pMesh = std::shared_ptr<En::Mesh>;
#endif // !MESH_HPP
