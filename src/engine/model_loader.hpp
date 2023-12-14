#pragma once
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "rendering/model.hpp"

namespace En {
class ModelLoader {
public:
  ModelLoader();

  Model load(pShader shader, std::string path);

private:
  void process_node(pShader shader, aiNode *node, const aiScene *scene,
                    const std::string &dir);

  std::vector<pMesh> meshes;
};

} // namespace En
