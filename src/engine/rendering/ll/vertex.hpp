#pragma once
#include <glm/glm.hpp>
#include <vector>

namespace En {
struct DataType {
  uint32_t count;
  void *offset;
};
using VertexDesc = std::vector<DataType>;

struct Vertex {
  Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex_coord)
      : pos(pos), normal(normal), tex_coord(tex_coord) {}

  static VertexDesc get_desc() {
    return {
        DataType{3, (void *)offsetof(Vertex, pos)},
        DataType{3, (void *)offsetof(Vertex, normal)},
        DataType{2, (void *)offsetof(Vertex, tex_coord)},
    };
  }
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 tex_coord;
};

struct VertexC {
  VertexC(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex_coord, glm::vec4 color)
      : pos(pos), normal(normal), tex_coord(tex_coord), color(color) {}
  VertexC(float x, float y, float z)
      : pos(x, y, z), normal(), tex_coord(), color(glm::vec4(1)) {}
  static VertexDesc get_desc() {
    return {
        DataType{3, (void *)offsetof(VertexC, pos)},
        DataType{3, (void *)offsetof(VertexC, normal)},
        DataType{2, (void *)offsetof(VertexC, tex_coord)},
        DataType{4, (void *)offsetof(VertexC, color)},
    };
  }
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 tex_coord;
  glm::vec4 color;
};

struct VertexPC {
  VertexPC(glm::vec3 pos, glm::vec4 color) : pos(pos), color(color) {}
  VertexPC(float x, float y, float z, float r, float g, float b, float a)
      : pos(glm::vec3(x, y, z)), color(glm::vec4(r, g, b, a)) {}

  static VertexDesc get_desc() {
    return {
        DataType{3, (void *)offsetof(VertexPC, pos)},
        DataType{4, (void *)offsetof(VertexPC, color)},
    };
  }

  glm::vec3 pos;
  glm::vec4 color;
};

struct VertexSkyBox {
  VertexSkyBox(glm::vec3 pos) : pos(pos) {}
  VertexSkyBox(float x, float y, float z) : pos(x, y, z) {}

  static VertexDesc get_desc() {
    return {
        DataType{3, (void *)offsetof(VertexSkyBox, pos)},
    };
  }

  glm::vec3 pos;
};

} // namespace En
