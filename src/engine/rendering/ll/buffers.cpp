#include "buffers.hpp"
#include <vector>

namespace En {

void VertexBuffer::init(void *vertices, uint32_t size, uint32_t elements,
                        VertexDesc desc, uint32_t vertex_size) {
  this->desc = desc;
  this->vertex_size = vertex_size;
  glGenBuffers(1, &this->id);

  this->bind();
  // TODO: Add more draw options (ex. GL_DYNAMIC_DRAW)
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  //    unbindVertexBuffer();
  this->count = elements / sizeof(float);
}

void VertexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, this->id); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::terminate() { glDeleteBuffers(1, &this->id); }

const VertexDesc VertexBuffer::get_desc() const { return this->desc; }
uint32_t VertexBuffer::get_vertex_size() const { return this->vertex_size; }

void IndexBuffer::init(uint32_t *indices, uint32_t size) {
  glCreateBuffers(1, &this->id);

  // GL_ELEMENT_ARRAY_BUFFER is not valid without an actively bound VAO
  // Binding with GL_ARRAY_BUFFER allows the data to be loaded regardless of VAO
  // state.
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
  this->elements = size / sizeof(uint32_t);
}

void IndexBuffer::bind() const { glBindBuffer(GL_ARRAY_BUFFER, this->id); }
void IndexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void IndexBuffer::terminate() { glDeleteBuffers(1, &this->id); }

} // namespace En
