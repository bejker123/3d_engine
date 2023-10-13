#ifndef BUFFER_H
#define BUFFER_H
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class VertexBuffer {
public:
  void init(void *vertices, uint32_t size, uint32_t elements);

  void bind();
  void unbind();

  void terminate();

public:
  uint32_t id;
  uint32_t count;
};

class IndexBuffer {
public:
  void init(uint32_t *indices, uint32_t size);

  void bind();
  void unbind();

  void terminate();
  uint32_t id;
  uint32_t elements;
};

#endif
