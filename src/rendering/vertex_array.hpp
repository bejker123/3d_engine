#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include "buffers.hpp"
#include <stdbool.h>

class VertexArray {
public:
  void init();

  void add_vertex_buffer(VertexBuffer *vb);

  void set_index_buffer(IndexBuffer *ib);

  void bind();

  void unbind();

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

#endif
