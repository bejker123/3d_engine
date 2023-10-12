#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include "buffers.hpp"
#include <stdbool.h>

class VertexArray {
public:
  void init();

  void addVertexBuffer(VertexBuffer *vb);

  void setIndexBuffer(IndexBuffer *ib);

  void bind();

  void unbind();

  GLuint getID();
  uint32_t getElements();

  // IndexBuffer* ib;
private:
  GLuint id;

  uint32_t vbi; // vertex buffer index

  bool inited;

  uint32_t elements, count;

  // VertexBuffer* vbs;
};

#endif
