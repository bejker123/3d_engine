#include "material.hpp"
#include "shader.hpp"
#include <GL/gl.h>
#include <memory>
#include <optional>

MaterialOptions::MaterialOptions() {
  this->cull_backfaces = false;
  this->polygon_mode = GL_FILL;
  this->depth = true;
  this->blend = true;
  this->multisample = true;
  this->stencil = true;
  this->texture = true;
}

bool MaterialOptions::set_polygon_mode(int mode) {
  if (mode == GL_POINT || mode == GL_LINE || mode == GL_FILL)
    this->polygon_mode = mode;
  else
    return false;
  return true;
}

void MaterialOptions::set_cull_backfaces(bool cull) {
  this->cull_backfaces = cull;
}

void MaterialOptions::bind() {

  glEnable(GL_TEXTURE_2D);
  // Set Front Face
  glFrontFace(GL_CCW);

  // Set Backface Culling
  if (this->cull_backfaces) {
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
  } else
    glDisable(GL_CULL_FACE);

  // Set Polygon Draw Mode
  glPolygonMode(GL_FRONT_AND_BACK, this->polygon_mode);

  // Set Depth Drawing Mode
  if (this->depth)
    glEnable(GL_DEPTH_TEST);
  else
    glDisable(GL_DEPTH_TEST);

  // Set Blending Mode
  if (this->blend) {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  } else
    glDisable(GL_BLEND);

  if (this->multisample)
    glEnable(GL_MULTISAMPLE);
  else
    glDisable(GL_MULTISAMPLE);

  if (this->stencil)
    glEnable(GL_STENCIL_TEST);
  else
    glDisable(GL_STENCIL_TEST);
}
void MaterialOptions::unbind() {
  glDisable(GL_CULL_FACE);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_DEPTH_TEST);

  glDisable(GL_BLEND);

  glDisable(GL_MULTISAMPLE);

  glDisable(GL_STENCIL_TEST);
}

void Material::init(std::shared_ptr<Shader> shader) {
  this->shader = shader;
  this->options = MaterialOptions();
  this->texture = std::nullopt;
}
void Material::bind() {
  this->options.bind();
  if (this->texture.has_value() && this->options.texture)
    this->texture.value()->bind(0);
  this->shader->set1i(0, "tex");
  this->shader->bind();
}
void Material::unbind() {
  MaterialOptions::unbind();
  Shader::unbind();
  Texture::unbind();
}

void Material::set_texture(std::shared_ptr<Texture> texture) {
  this->texture = texture;
}
std::shared_ptr<Shader> Material::get_shader() const { return this->shader; }
MaterialOptions *Material::get_options() { return &this->options; }
