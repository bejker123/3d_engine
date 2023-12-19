#include "mesh.hpp"
#include "ll/buffers.hpp"
#include "ll/vertex_array.hpp"
#include <cstdint>
#include <iostream>

namespace En {

void Mesh::render() const {
  this->mat->bind();
  for (auto i : this->vas) {
    i->bind();
    if (i->get_elements() >
        0) // If a buffer va doesn't contain an ib draw just the vertices
      glDrawElements(GL_TRIANGLES, i->get_elements(), GL_UNSIGNED_INT, 0);
    else
      glDrawArrays(GL_TRIANGLES, 0, i->get_count());
    i->unbind();
  }
  this->mat->unbind();
}

Mesh::Mesh(pMaterial material, std::vector<Vertex> vertices,
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
Mesh::Mesh(pMaterial material, std::vector<VertexC> vertices,
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

Mesh::Mesh(pMaterial material, std::vector<VertexC> vertices, uint32_t *indices,
           uint32_t size)
    : mat(material) {
  VertexArray va;
  VertexBuffer vb;
  IndexBuffer ib;
  va.init();
  vb.init(vertices);
  ib.init(indices, size);
  va.add_vertex_buffer(&vb);
  va.set_index_buffer(&ib);
  this->vas.push_back(std::make_shared<VertexArray>(va));
}

Mesh::Mesh(pMaterial material, pVertexArray va) {
  this->vas.push_back(va);
  this->mat = material;
};
void Mesh::init(pMaterial material, pVertexArray va) {
  this->vas.push_back(va);
  this->mat = material;
};

pMaterial Mesh::get_material() { return this->mat; }
} // namespace En
