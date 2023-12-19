#include "vertex_array.hpp"
#include "../../io/logger.hpp"
#include "buffers.hpp"
#include <cstdio>

namespace En {

void VertexArray::init() {
  glGenVertexArrays(1, &this->id);
  glBindVertexArray(this->id);
  this->elements = 0;
  this->count = 0;
  this->vbi = 0;
  this->inited = true;
}

void VertexArray::add_vertex_buffer(VertexBuffer *vb) {
  assert(this->inited == true);

  this->bind();
  vb->bind();

  auto vb_desc = vb->get_desc();
  auto size = vb->get_vertex_size();

  for (auto &i : vb_desc) {
    glVertexAttribPointer(this->vbi, i.count, GL_FLOAT, GL_FALSE, size,
                          i.offset);
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;
  }

  this->unbind();
  vb->unbind();

  this->count += vb->count;
}

void VertexArray::set_index_buffer(IndexBuffer *ib) {
  assert(this->inited == true);
  this->bind();
  ib->bind();
  ib->unbind();
  this->unbind();
  this->elements = ib->elements;
}

void VertexArray::bind() const { glBindVertexArray(this->id); }
void VertexArray::unbind() { glBindVertexArray(0); }

GLuint VertexArray::get_id() const { return this->id; }
uint32_t VertexArray::get_elements() const { return this->elements; }
uint32_t VertexArray::get_count() const { return this->count; }

} // namespace En
