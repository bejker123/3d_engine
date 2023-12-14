#ifndef MESH_HPP
#define MESH_HPP
#include "ll/vertex_array.hpp"
#include <memory>
#include <vector>

namespace En {

class Mesh {
public:
  void init(std::shared_ptr<ll::VertexArray> va);

  Mesh(std::vector<ll::Vertex> vertices, std::vector<unsigned int> indices);
  Mesh(std::vector<ll::VertexC> vertices, std::vector<unsigned int> indices);
  Mesh(){};
  void add_va(std::shared_ptr<ll::VertexArray> va);

  void render();

private:
  std::vector<std::shared_ptr<ll::VertexArray>> vas;
};

} // namespace En
#endif // !MESH_HPP
