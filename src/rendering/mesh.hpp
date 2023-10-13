#ifndef MESH
#define MESH
#include "vertex_array.hpp"
#include <memory>
#include <vector>

class Mesh {
public:
  void init(std::shared_ptr<VertexArray> va) { this->vas.push_back(va); };
  void addVA(std::shared_ptr<VertexArray> va);

  void render();

private:
  std::vector<std::shared_ptr<VertexArray>> vas;
};

#endif // !MESH
