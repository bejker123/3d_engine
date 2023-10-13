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
  bool setPolygonMode(int mode) {
    if (mode == GL_POINT || mode == GL_LINE || mode == GL_FILL)
      this->polygon_mode = mode;
    else
      return false;
    return true;
  }

  void setCullBackfaces(bool cull) { this->cull_backfaces = cull; }

private:
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

  std::shared_ptr<Shader> getShader() const { return this->shader; }
  MaterialOptions *getOptions() { return &this->options; }

private:
  std::shared_ptr<Shader> shader;
  std::shared_ptr<Texture> texture;
  MaterialOptions options;
};
#endif // !MATERIAL_HPP
