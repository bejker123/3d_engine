#pragma once
#include "ll/vertex_array.hpp"
#include "material.hpp"
#include <memory>
#include <vector>

namespace En {

class Mesh {
public:
  void init(pMaterial material, pVertexArray va);

  Mesh(pMaterial material, std::vector<Vertex> vertices,
       std::vector<unsigned int> indices);
  Mesh(pMaterial material, std::vector<VertexC> vertices,
       std::vector<unsigned int> indices);

  Mesh(pMaterial material, std::vector<VertexC> vertices, uint32_t *indices,
       uint32_t size);

  Mesh(pMaterial material, pVertexArray va);
  Mesh(){};
  void add_va(pVertexArray va);
  pMaterial get_material();

  void render() const;

private:
  std::vector<pVertexArray> vas;
  pMaterial mat;
};

} // namespace En
using pMesh = std::shared_ptr<En::Mesh>;
