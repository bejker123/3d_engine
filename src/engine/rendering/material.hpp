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
  void bind();
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
  void init(std::shared_ptr<const ll::Shader> shader);
  void bind();
  void unbind();

  void set_texture(const std::shared_ptr<ll::Texture> texture);
  void set_shader(const std::shared_ptr<const ll::Shader> shader);
  std::shared_ptr<const ll::Shader> get_shader() const;
  MaterialOptions *get_options();
  std::optional<std::shared_ptr<ll::Texture>> get_texture();

private:
  std::shared_ptr<const ll::Shader> shader;
  std::optional<std::shared_ptr<ll::Texture>> texture;
  MaterialOptions options;
  uint32_t texture_bind_idx = 0;
};
} // namespace En
#endif // !MATERIAL_HPP
