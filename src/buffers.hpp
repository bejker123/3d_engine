#ifndef BUFFER_H
#define BUFFER_H
#include<GL/glew.h>
#include<GLFW/glfw3.h>

struct VertexBuffer{
    uint32_t id;
    uint32_t count;
};

typedef struct VertexBuffer VertexBuffer;

void initVertexBuffer(VertexBuffer* vb,void* vertices, uint32_t size,uint32_t elements);

void bindVertexBuffer(VertexBuffer* vb);
void unbindVertexBuffer();

void terminateVertexBuffer(VertexBuffer* vb);

struct IndexBuffer{
    uint32_t id;
    uint32_t elements;
};

typedef struct IndexBuffer IndexBuffer;

void initIndexBuffer(IndexBuffer* ib,uint32_t* indices, uint32_t size);

void bindIndexBuffer(IndexBuffer* ib);
void unbindIndexBuffer();

void terminateIndexBuffer(IndexBuffer* ib);

#endif
