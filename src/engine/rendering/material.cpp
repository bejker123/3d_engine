#include "material.hpp"
#include "ll/shader.hpp"
#include <GL/gl.h>
#include <memory>
#include <optional>

En::MaterialOptions::MaterialOptions() {
  this->cull_backfaces = true;
  this->polygon_mode = GL_FILL;
  this->depth = true;
  this->blend = true;
  this->multisample = true;
  this->stencil = true;
  this->texture = true;
}

bool En::MaterialOptions::set_polygon_mode(int mode) {
  if (mode == GL_POINT || mode == GL_LINE || mode == GL_FILL)
    this->polygon_mode = mode;
  else
    return false;
  return true;
}

void En::MaterialOptions::set_cull_backfaces(bool cull) {
  this->cull_backfaces = cull;
}

void En::MaterialOptions::bind() {

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
void En::MaterialOptions::unbind() {
  glDisable(GL_CULL_FACE);

  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  glDisable(GL_DEPTH_TEST);

  glDisable(GL_BLEND);

  glDisable(GL_MULTISAMPLE);

  glDisable(GL_STENCIL_TEST);
}

void En::Material::init(std::shared_ptr<const ll::Shader> shader) {
  this->shader = shader;
  this->options = MaterialOptions();
  this->texture = std::nullopt;
}
void En::Material::bind() {
  this->options.bind();
  if (this->texture.has_value() && this->options.texture) {
    this->texture.value()->bind(texture_bind_idx);
    this->shader->set1i(texture_bind_idx, "tex");
  }
  this->shader->bind();
}
void En::Material::unbind() {
  En::MaterialOptions::unbind();
  ll::Shader::unbind();
  ll::Texture::unbind();
}

void En::Material::set_texture(const std::shared_ptr<ll::Texture> texture) {
  this->texture = texture;
}

void En::Material::set_shader(const std::shared_ptr<const ll::Shader> shader) {
  this->shader = shader;
}
std::shared_ptr<const En::ll::Shader> En::Material::get_shader() const {
  return this->shader;
}
En::MaterialOptions *En::Material::get_options() { return &this->options; }
