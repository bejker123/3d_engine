#include "vertex_array.hpp"
#include "../../io/logger.hpp"
#include "buffers.hpp"
#include <cstdio>

namespace En {
namespace ll {

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

  auto vb_type = vb->get_type();

  if (vb_type == VertexType::POSx3F_COLORx4F) {
    // position
    glVertexAttribPointer(this->vbi, sizeof(VertexPC::pos) / sizeof(float),
                          GL_FLOAT, GL_FALSE, sizeof(VertexPC),
                          (void *)offsetof(VertexPC, pos));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    // color
    glVertexAttribPointer(this->vbi, sizeof(VertexPC::color) / sizeof(float),
                          GL_FLOAT, false, sizeof(VertexPC),
                          (void *)offsetof(VertexPC, color));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;
  } else if (vb_type == POSx3F_NORMx3F_TEXx2F) {
    // position
    glVertexAttribPointer(this->vbi, sizeof(Vertex::pos) / sizeof(float),
                          GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (void *)offsetof(Vertex, pos));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    // normal
    glVertexAttribPointer(this->vbi, sizeof(Vertex::normal) / sizeof(float),
                          GL_FLOAT, false, sizeof(Vertex),
                          (void *)offsetof(Vertex, normal));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    // tex coord
    glVertexAttribPointer(this->vbi, sizeof(Vertex::tex_coord) / sizeof(float),
                          GL_FLOAT, false, sizeof(Vertex),
                          (void *)offsetof(Vertex, tex_coord));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;
  } else if (vb_type == POSx3F_NORMx3F_TEXx2F_COLx4F) {
    // position
    glVertexAttribPointer(this->vbi, sizeof(VertexC::pos) / sizeof(float),
                          GL_FLOAT, GL_FALSE, sizeof(VertexC),
                          (void *)offsetof(VertexC, pos));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    // normal
    glVertexAttribPointer(this->vbi, sizeof(VertexC::normal) / sizeof(float),
                          GL_FLOAT, false, sizeof(VertexC),
                          (void *)offsetof(VertexC, normal));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    // tex coord
    glVertexAttribPointer(this->vbi, sizeof(VertexC::tex_coord) / sizeof(float),
                          GL_FLOAT, false, sizeof(VertexC),
                          (void *)offsetof(VertexC, tex_coord));
    glEnableVertexAttribArray(this->vbi);
    this->vbi++;

    glVertexAttribPointer(this->vbi, sizeof(VertexC::color) / sizeof(float),
                          GL_FLOAT, false, sizeof(VertexC),
                          (void *)offsetof(VertexC, color));
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

void VertexArray::bind() { glBindVertexArray(this->id); }
void VertexArray::unbind() { glBindVertexArray(0); }

GLuint VertexArray::get_id() const { return this->id; }
uint32_t VertexArray::get_elements() const { return this->elements; }
uint32_t VertexArray::get_count() const { return this->count; }

} // namespace ll
} // namespace En
