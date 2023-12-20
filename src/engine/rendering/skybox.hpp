#pragma once
#include "ll/vertex.hpp"
#include "material.hpp"
#include "mesh.hpp"
namespace En {
class SkyBox {
public:
  SkyBox() {}
  void init(pShader shader, std::vector<std::string> textures) {
    auto material = std::make_shared<Material>(
        Material(shader, std::make_shared<Texture>(Texture(textures))));
    material->get_options()->depth = false;
    material->get_options()->cull_backfaces = false;
    this->mesh = Mesh(material, this->skybox_vertices, std::vector<uint32_t>());
  }

  void render() { this->mesh.render(); }
  pShader get_shader() { return this->mesh.get_material()->get_shader(); }

private:
  Mesh mesh;

  const std::vector<VertexSkyBox> skybox_vertices = {
      VertexSkyBox(-1.0f, 1.0f, -1.0f),  VertexSkyBox(-1.0f, -1.0f, -1.0f),
      VertexSkyBox(1.0f, -1.0f, -1.0f),  VertexSkyBox(1.0f, -1.0f, -1.0f),
      VertexSkyBox(1.0f, 1.0f, -1.0f),   VertexSkyBox(-1.0f, 1.0f, -1.0f),
      VertexSkyBox(-1.0f, -1.0f, 1.0f),  VertexSkyBox(-1.0f, -1.0f, -1.0f),
      VertexSkyBox(-1.0f, 1.0f, -1.0f),  VertexSkyBox(-1.0f, 1.0f, -1.0f),
      VertexSkyBox(-1.0f, 1.0f, 1.0f),   VertexSkyBox(-1.0f, -1.0f, 1.0f),
      VertexSkyBox(1.0f, -1.0f, -1.0f),  VertexSkyBox(1.0f, -1.0f, 1.0f),
      VertexSkyBox(1.0f, 1.0f, 1.0f),    VertexSkyBox(1.0f, 1.0f, 1.0f),
      VertexSkyBox(1.0f, 1.0f, -1.0f),   VertexSkyBox(1.0f, -1.0f, -1.0f),
      VertexSkyBox(-1.0f, -1.0f, 1.0f),  VertexSkyBox(-1.0f, 1.0f, 1.0f),
      VertexSkyBox(1.0f, 1.0f, 1.0f),    VertexSkyBox(1.0f, 1.0f, 1.0f),
      VertexSkyBox(1.0f, -1.0f, 1.0f),   VertexSkyBox(-1.0f, -1.0f, 1.0f),
      VertexSkyBox(-1.0f, 1.0f, -1.0f),  VertexSkyBox(1.0f, 1.0f, -1.0f),
      VertexSkyBox(1.0f, 1.0f, 1.0f),    VertexSkyBox(1.0f, 1.0f, 1.0f),
      VertexSkyBox(-1.0f, 1.0f, 1.0f),   VertexSkyBox(-1.0f, 1.0f, -1.0f),
      VertexSkyBox(-1.0f, -1.0f, -1.0f), VertexSkyBox(-1.0f, -1.0f, 1.0f),
      VertexSkyBox(1.0f, -1.0f, -1.0f),  VertexSkyBox(1.0f, -1.0f, -1.0f),
      VertexSkyBox(-1.0f, -1.0f, 1.0f),  VertexSkyBox(1.0f, -1.0f, 1.0f)};
};
} // namespace En
