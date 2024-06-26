#pragma once
#include "buffers.hpp"
#include <memory>

namespace En {

class VertexArray {
public:
  void init();

  void add_vertex_buffer(VertexBuffer *vb);

  void set_index_buffer(IndexBuffer *ib);

  void bind() const;

  static void unbind();

  GLuint get_id() const;
  uint32_t get_elements() const;
  uint32_t get_count() const;

  // IndexBuffer* ib;
private:
  GLuint id;

  uint32_t vbi; // vertex buffer index

  bool inited;

  uint32_t elements, count;

  // VertexBuffer* vbs;
};
} // namespace En
using pVertexArray = std::shared_ptr<const En::VertexArray>;
