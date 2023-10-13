#include "material.hpp"
#include <memory>

MaterialOptions::MaterialOptions() { this->cull_backfaces = false; }

void MaterialOptions::bind() {
  if (this->cull_backfaces) {
    // backface culling
    glCullFace(GL_BACK);
    glEnable(GL_CULL_FACE);
  } else {
    glDisable(GL_CULL_FACE);
  }
}
void MaterialOptions::unbind() {
  glDisable(GL_MULTISAMPLE);
  glDisable(GL_BLEND);
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
