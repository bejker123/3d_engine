#include "material.hpp"
#include "ll/shader.hpp"
#include <GL/gl.h>
#include <iostream>
#include <memory>
#include <optional>

namespace En {

MaterialOptions::MaterialOptions() {
  this->cull_backfaces = true;
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

void MaterialOptions::bind() const {

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
  if (this->depth) {
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
  } else
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
  glDepthMask(GL_FALSE);
}

Material::Material(pShader shader, const pTexture texture) {
  this->shader = shader;
  this->options = MaterialOptions();
  this->texture = texture;
}

Material::Material(pShader shader, const pTexture texture,
                   const pTexture specular) {
  this->shader = shader;
  this->options = MaterialOptions();
  this->texture = texture;
  this->specular = specular;
}

void Material::init(pShader shader) {
  this->shader = shader;
  this->options = MaterialOptions();
  this->texture = std::nullopt;
}
void Material::bind() const {
  this->options.bind();
  if (this->texture.has_value() && this->options.texture) {
    this->texture.value()->bind(texture_bind_idx);
    this->shader->set1i(texture_bind_idx, "tex");
  }
  if (this->specular.has_value() && this->options.texture) {
    this->specular.value()->bind(texture_bind_idx + 1);
    this->shader->set1i(texture_bind_idx + 1, "specular");
  }
  this->shader->bind();
}
void Material::unbind() const {
  MaterialOptions::unbind();
  Shader::unbind();
  Texture::unbind();
}

void Material::set_texture(const pTexture texture) { this->texture = texture; }

void Material::set_shader(const pShader shader) { this->shader = shader; }
pShader Material::get_shader() const { return this->shader; }
MaterialOptions *Material::get_options() { return &this->options; }
std::optional<pTexture> Material::get_texture() { return this->texture; }

} // namespace En
