#include "material.hpp"
#include <memory>

MaterialOptions::MaterialOptions() {
  this->cull_backfaces = false;
  this->polygon_mode = GL_FILL;
}

void MaterialOptions::bind() {
  if (this->cull_backfaces) {
    // backface culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else {
    glDisable(GL_CULL_FACE);
  }
  glPolygonMode(GL_FRONT_AND_BACK, this->polygon_mode);
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
void MaterialOptions::unbind() {
  glDisable(GL_MULTISAMPLE);
  glDisable(GL_BLEND);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Material::init(std::shared_ptr<Shader> shader) {
  this->shader = shader;
  this->options = MaterialOptions();
}
void Material::bind() {
  this->options.bind();
  this->shader->bind();
  // this->texture->bind();
}
void Material::unbind() {
  this->options.unbind();
  this->shader->unbind();
  // this->texture->unbind();
}
