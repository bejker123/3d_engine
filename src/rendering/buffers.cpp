#include "buffers.hpp"
#include "../logger.hpp"
#include <vector>

void VertexBuffer::init(void *vertices, uint32_t size, uint32_t elements,
                        VertexType type) {
  this->type = type;
  glGenBuffers(1, &this->id);

  this->bind();
  // TODO: Add more draw options (ex. GL_DYNAMIC_DRAW)
  glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
  //    unbindVertexBuffer();
  this->count = elements / sizeof(float);
}
void VertexBuffer::init(std::vector<Vertex> vertices) {
  this->type = VertexType::POSx3F_NORMx3F_TEXx2F;
  glGenBuffers(1, &this->id);

  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);
  this->count = vertices.size();
}

void VertexBuffer::init(std::vector<VertexPC> vertices) {
  this->type = VertexType::POSx3F_COLORx4F;
  glGenBuffers(1, &this->id);

  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexPC),
               vertices.data(), GL_STATIC_DRAW);
  this->count = vertices.size();
}
void VertexBuffer::init(std::vector<VertexC> vertices) {
  this->type = VertexType::POSx3F_NORMx3F_TEXx2F_COLx4F;
  glGenBuffers(1, &this->id);

  this->bind();
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexC),
               vertices.data(), GL_STATIC_DRAW);
  this->count = vertices.size();
}

void VertexBuffer::bind() { glBindBuffer(GL_ARRAY_BUFFER, this->id); }
void VertexBuffer::unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void VertexBuffer::terminate() { glDeleteBuffers(1, &this->id); }

const VertexType VertexBuffer::get_type() const { return this->type; }

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
