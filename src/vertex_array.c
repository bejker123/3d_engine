#include"vertex_array.h"
#include"logger.h"

VertexArray initVertexArray()
{
  VertexArray ret;
  glGenVertexArrays(1, &ret.id);
  glBindVertexArray(ret.id);
  ret.elements = 0;
  ret.count = 0;
  ret.vbi = 0;
  ret.inited = true;
  return ret;
}

void addVertexBuffer(VertexArray* va,VertexBuffer* vb){
  assert(va->inited == true);

  bindVertexArray(va);
  bindVertexBuffer(vb);

  // set the vertex attributes pointers
		//pos
		glVertexAttribPointer(va->vbi, vb->count, GL_FLOAT, GL_FALSE, sizeof(float) * vb->count, (void*)0);
		glEnableVertexAttribArray(va->vbi);
		va->vbi++;

		//color
		//glVertexAttribPointer(va->vbi, sizeof(Vertex::color) / sizeof(float), GL_FLOAT, normalised, sizeof(Vertex), (void*)offsetof(Vertex, color));
		//glEnableVertexAttribArray(va->vbi);
		//va->vbi++;

		//texture coords
		/*glVertexAttribPointer(va->vbi, sizeof(Vertex::tex_coord) / sizeof(float), GL_FLOAT, normalised, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));
		glEnableVertexAttribArray(va->vbi);
		va->vbi++;
    */

  unbindVertexArray();
  unbindVertexBuffer();

  va->count += vb->count;

}

void setIndexBuffer(VertexArray* va,IndexBuffer* ib)
{
    assert(va->inited == true);
    bindVertexArray(va);
    bindIndexBuffer(ib);
    unbindIndexBuffer();
    unbindVertexArray();
    //LOG_VAR("%d",ib->elements);
    va->elements = ib->elements;
    //printf("va->ib == ib ? %d\n",(va->ib->id == ib->id)&&(va->ib->elements == ib->elements));
}

void bindVertexArray(VertexArray* va)
{
    glBindVertexArray(va->id);
}

void unbindVertexArray()
{
    glBindVertexArray(0);
}

