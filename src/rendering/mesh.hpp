#ifndef MESH
#define MESH
#include "ll/vertex_array.hpp"
#include <memory>
#include <vector>

class Mesh {
public:
  void init(std::shared_ptr<VertexArray> va);

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
  Mesh(){};
  void add_va(std::shared_ptr<VertexArray> va);
  void load(std::string path);

  void render();

private:
  std::vector<std::shared_ptr<VertexArray>> vas;
};

#endif // !MESH
