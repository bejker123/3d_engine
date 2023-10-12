#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H
#include"buffers.h"
#include<stdbool.h>

struct VertexArray{
  //IndexBuffer* ib;

  GLuint id;

	uint32_t vbi;//vertex buffer index
	
  bool inited;

  uint32_t elements,count;

  //VertexBuffer* vbs;
};

typedef struct VertexArray VertexArray;

void addVertexBuffer(VertexArray* va,VertexBuffer* vb);

void setIndexBuffer(VertexArray* va,IndexBuffer* ib);

VertexArray initVertexArray();

void bindVertexArray(VertexArray* va);

void unbindVertexArray();

#endif
