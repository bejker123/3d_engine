#include "mesh.hpp"
#include "buffers.hpp"
#include "vertex_array.hpp"

void Mesh::render() {
  for (auto i : this->vas) {
    i->bind();
    glDrawElements(GL_TRIANGLES, i->getElements(), GL_UNSIGNED_INT, 0);
    i->unbind();
  }
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
  VertexArray va;
  va.init();
  VertexBuffer vb;
  vb.init(vertices);
  va.addVertexBuffer(&vb);
}
