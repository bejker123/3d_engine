#ifndef BUFFER_H
#define BUFFER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <vector>

struct VertexPC {
public:
  VertexPC(glm::vec3 pos, glm::vec4 color) : pos(pos), color(color) {}
  VertexPC(float x, float y, float z, float r, float g, float b, float a)
      : pos(glm::vec3(x, y, z)), color(glm::vec4(r, g, b, a)) {}

  glm::vec3 pos;
  glm::vec4 color;
};

struct Vertex {
public:
  Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 tex_coord)
      : pos(pos), normal(normal), tex_coord(tex_coord) {}
  glm::vec3 pos;
  glm::vec3 normal;
  glm::vec2 tex_coord;
};

enum VertexType {
  POSx3F_NORMx3F_TEXx2F, // vec3 pos; vec3 normal; vec2 tex_coord
  POSx3F_COLORx4F        // vec3 pos; vec4 color
};

class VertexBuffer {
public:
  void init(void *vertices, uint32_t size, uint32_t elements, VertexType type);
  void init(std::vector<Vertex> vertices);
  void init(std::vector<VertexPC> vertices);

  void bind();
  void unbind();

  void terminate();

  const VertexType get_type() const;

public:
  VertexType type;
  uint32_t id;
  uint32_t count;
};

class IndexBuffer {
public:
  void init(uint32_t *indices, uint32_t size);

  void bind();
  void unbind();

  void terminate();
  uint32_t id;
  uint32_t elements;
};

#endif
