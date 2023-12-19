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
