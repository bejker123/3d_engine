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

private:
  bool cull_backfaces;
  int polygon_mode; // GL_POINT; GL_LINE; GL_FILL
};

class Material {
public:
  void init(std::shared_ptr<Shader> shader);
  void bind();
  void unbind();

  std::shared_ptr<Shader> getShader() const { return this->shader; }

private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> texture;
  MaterialOptions options;
};
#endif // !MATERIAL_HPP
