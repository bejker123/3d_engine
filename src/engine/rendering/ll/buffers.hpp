#pragma once
#include "vertex.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace En {

class VertexBuffer {
public:
  // Implementation has to be in header
  void init(void *vertices, uint32_t size, uint32_t elements, VertexDesc desc,
            uint32_t vertex_size);
  template <class T> void init(std::vector<T> vertices) {
    this->desc = T::get_desc();
    this->vertex_size = sizeof(T);
    glGenBuffers(1, &this->id);

    this->bind();
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(),
                 GL_STATIC_DRAW);
    this->count = vertices.size();
  }

  void bind() const;
  static void unbind();

  void terminate();

  const VertexDesc get_desc() const;
  uint32_t get_vertex_size() const;

public:
  VertexDesc desc;
  uint32_t id;
  uint32_t count;
  uint32_t vertex_size;
};

class IndexBuffer {
public:
  void init(uint32_t *indices, uint32_t size);

  void bind() const;
  static void unbind();

  void terminate();
  uint32_t id;
  uint32_t elements;
};

} // namespace En
