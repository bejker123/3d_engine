#include "buffers.hpp"
#include "../logger.hpp"

void VertexBuffer::init(void *vertices, uint32_t size, uint32_t elements) {
  glGenBuffers(1, &this->id);

  this->bind();
  // TODO: Add more draw options (ex. GL_DYNAMIC_DRAW)
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  //    unbindVertexBuffer();
  this->count = elements / sizeof(float);
}

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, this->id); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::terminate() { glDeleteBuffers(1, &this->id); }

void IndexBuffer::init(uint32_t *indices, uint32_t size) {
  glCreateBuffers(1, &this->id);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO
  // state.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  this->elements = size / sizeof(uint32_t);
  LOG_VAR("%i", this->elements);
}

void IndexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, this->id); }
void IndexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void IndexBuffer::terminate() { glDeleteBuffers(1, &this->id); }