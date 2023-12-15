#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "ll/shader.hpp"
#include "ll/texture.hpp"
#include <memory>
#include <optional>

namespace En {

class MaterialOptions {
public:
  MaterialOptions();
  void bind() const;
  static void unbind();
  // Valid modes: GL_POINT GL_LINE GL_FILL
  // Return true if set succesfully
  bool set_polygon_mode(int mode);

  void set_cull_backfaces(bool cull);

  // private:
  bool cull_backfaces;
  int polygon_mode; // GL_POINT; GL_LINE; GL_FILL
  bool depth;
  bool blend;
  bool multisample;
  bool stencil;
  bool texture;
};

class Material {
public:
  Material(){};
  Material(pShader shader, pTexture texture);
  void init(pShader shader);
  void bind() const;
  void unbind() const;

  void set_texture(const pTexture texture);
  void set_shader(const pShader shader);
  pShader get_shader() const;
  MaterialOptions *get_options();
  std::optional<pTexture> get_texture();

private:
  pShader shader;
  std::optional<pTexture> texture;
  MaterialOptions options;
  uint32_t texture_bind_idx = 0;
};
} // namespace En
using pMaterial = std::shared_ptr<En::Material>;
#endif // !MATERIAL_HPP
