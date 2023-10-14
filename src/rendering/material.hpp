#ifndef MATERIAL_HPP
#define MATERIAL_HPP
#include "shader.hpp"
#include "texture.hpp"
#include <memory>
class MaterialOptions {
public:
  MaterialOptions();
  void bind();
  void unbind();

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
};

class Material {
public:
  void init(std::shared_ptr<Shader> shader);
  void bind();
  void unbind();

  void set_texture(std::shared_ptr<Texture> texture);
  std::shared_ptr<Shader> get_shader() const;
  MaterialOptions *get_options();

private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> texture;
  MaterialOptions options;
};
#endif // !MATERIAL_HPP
