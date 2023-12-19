#pragma once
#include "ll/vertex_array.hpp"
#include "material.hpp"
#include <memory>
#include <vector>

namespace En {

class Mesh {
public:
  void init(pMaterial material, pVertexArray va);

  // Implementation has to be in header
  template <class T>
  Mesh(pMaterial material, std::vector<T> vertices,
       std::vector<unsigned int> indices)
      : mat(material) {
    VertexArray va;
    VertexBuffer vb;
    IndexBuffer ib;
    va.init();
    vb.init(vertices);
    ib.init(indices.data(), indices.size() * sizeof(uint32_t));
    va.add_vertex_buffer(&vb);
    va.set_index_buffer(&ib);
    this->vas.push_back(std::make_shared<VertexArray>(va));
  }

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
