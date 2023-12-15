#ifndef MESH_HPP
#define MESH_HPP
#include "ll/vertex_array.hpp"
#include "material.hpp"
#include <memory>
#include <vector>

namespace En {

class Mesh {
public:
  void init(pMaterial material, pVertexArray va);

  Mesh(pMaterial material, std::vector<ll::Vertex> vertices,
       std::vector<unsigned int> indices);
  Mesh(pMaterial material, std::vector<ll::VertexC> vertices,
       std::vector<unsigned int> indices);

  Mesh(pMaterial material, pVertexArray va);
  Mesh(){};
  void add_va(pVertexArray va);
  pMaterial get_material();

  void render();

private:
  std::vector<pVertexArray> vas;
  pMaterial mat;
};

} // namespace En
using pMesh = std::shared_ptr<En::Mesh>;
#endif // !MESH_HPP
