#include "buffers.hpp"
#include "logger.hpp"

void initVertexBuffer(VertexBuffer *vb, void *vertices, uint32_t size,
                      uint32_t elements) {
  glGenBuffers(1, &vb->id);

  bindVertexBuffer(vb);
  // TODO: Add more draw options (ex. GL_DYNAMIC_DRAW)
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  //    unbindVertexBuffer();
  vb->count = elements / sizeof(float);
}

void bindVertexBuffer(VertexBuffer *vb) {
  glBindBuffer(GL_ARRAY_BUFFER, vb->id);
}
void unbindVertexBuffer() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void terminateVertexBuffer(VertexBuffer *iv) { glDeleteBuffers(1, &iv->id); }

void initIndexBuffer(IndexBuffer *ib, uint32_t *indices, uint32_t size) {
  glCreateBuffers(1, &ib->id);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO
  // state.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  ib->elements = size / sizeof(uint32_t);
  LOG_VAR("%i", ib->elements);
}

void bindIndexBuffer(IndexBuffer *ib) { glBindBuffer(GL_ARRAY_BUFFER, ib->id); }
void unbindIndexBuffer() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void terminateIndexBuffer(IndexBuffer *ib) { glDeleteBuffers(1, &ib->id); }
