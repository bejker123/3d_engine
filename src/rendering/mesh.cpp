#include "mesh.hpp"
#include "buffers.hpp"
#include "vertex_array.hpp"

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

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
  VertexArray va;
  va.init();
  VertexBuffer vb;
  vb.init(vertices);
  va.add_vertex_buffer(&vb);
}
void Mesh::init(std::shared_ptr<VertexArray> va) { this->vas.push_back(va); };
