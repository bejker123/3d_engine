#include "mesh.hpp"

void Mesh::render() {
  for (auto i : this->vas) {
    i->bind();
    glDrawElements(GL_TRIANGLES, i->getElements(), GL_UNSIGNED_INT, 0);
    i->unbind();
  }
}
