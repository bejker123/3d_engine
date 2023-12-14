#include "mesh.hpp"
#include "ll/buffers.hpp"
#include "ll/vertex_array.hpp"
#include <iostream>

namespace En {

void Mesh::render() {
  for (auto i : this->vas) {
    i->bind();
    if (i->get_elements() >
        0) // If a buffer va doesn't contain an ib draw just the vertices
      glDrawElements(GL_TRIANGLES, i->get_elements(), GL_UNSIGNED_INT, 0);
    else
      glDrawArrays(GL_TRIANGLES, 0, i->get_count());
    i->unbind();
  }
}

Mesh::Mesh(std::vector<ll::Vertex> vertices,
           std::vector<unsigned int> indices) {
  ll::VertexArray va;
  va.init();
  ll::VertexBuffer vb;
  vb.init(vertices);
  va.add_vertex_buffer(&vb);
  this->vas.push_back(std::make_shared<ll::VertexArray>(va));
}
Mesh::Mesh(std::vector<ll::VertexC> vertices,
           std::vector<unsigned int> indices) {
  ll::VertexArray va;
  va.init();
  ll::VertexBuffer vb;
  vb.init(vertices);
  va.add_vertex_buffer(&vb);
  this->vas.push_back(std::make_shared<ll::VertexArray>(va));
}
void Mesh::init(std::shared_ptr<ll::VertexArray> va) {
  this->vas.push_back(va);
};

} // namespace En
