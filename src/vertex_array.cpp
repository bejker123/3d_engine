#include "vertex_array.hpp"
#include "logger.hpp"
#include <cstdio>

void VertexArray::init() {
  glGenVertexArrays(1, &this->id);
  glBindVertexArray(this->id);
  this->elements = 0;
  this->count = 0;
  this->vbi = 0;
  this->inited = true;
  printf("DBG VA ID: %d", this->id);
}

void VertexArray::addVertexBuffer(VertexBuffer *vb) {
  assert(this->inited == true);

  this->bind();
  vb->bind();

  // set the vertex attributes pointers
  // pos
  glVertexAttribPointer(this->vbi, vb->count, GL_FLOAT, GL_FALSE,
                        sizeof(float) * vb->count, (void *)0);
  glEnableVertexAttribArray(this->vbi);
  this->vbi++;

  // color
  // glVertexAttribPointer(this->vbi, sizeof(Vertex::color) / sizeof(float),
  // GL_FLOAT, normalised, sizeof(Vertex), (void*)offsetof(Vertex, color));
  // glEnableVertexAttribArray(this->vbi);
  // this->vbi++;

  // texture coords
  /*glVertexAttribPointer(this->vbi, sizeof(Vertex::tex_coord) / sizeof(float),
  GL_FLOAT, normalised, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
  glEnableVertexAttribArray(this->vbi);
  this->vbi++;
*/

  this->unbind();
  vb->unbind();

  this->count += vb->count;
}

void VertexArray::setIndexBuffer(IndexBuffer *ib) {
  assert(this->inited == true);
  this->bind();
  ib->bind();
  ib->unbind();
  this->unbind();
  // LOG_thisR("%d",ib->elements);
  this->elements = ib->elements;
  // printf("this->ib == ib ? %d\n",(this->ib->id ==
  // ib->id)&&(this->ib->elements == ib->elements));
}

void VertexArray::bind() { glBindVertexArray(this->id); }

void VertexArray::unbind() { glBindVertexArray(0); }

GLuint VertexArray::getID() { return this->id; }
uint32_t VertexArray::getElements() { return this->elements; }
