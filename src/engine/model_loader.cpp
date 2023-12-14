#include "model_loader.hpp"
#include <iostream>
namespace En {

ModelLoader::ModelLoader() {}

ll::Texture load_texture(aiMaterial *mat, aiTextureType type,
                         std::string directory) {
  // for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
  aiString str;
  // mat->GetTexture(type, i, &str);
  mat->GetTexture(type, 0, &str);
  return ll::Texture(directory + str.C_Str());
  // }
}

Mesh process_mesh(pShader shader, aiMesh *mesh, const aiScene *scene,
                  const std::string &dir) {
  std::vector<ll::VertexC> vertices;
  std::vector<unsigned int> indices;

  for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
    auto vert = mesh->mVertices[i];
    auto norm = mesh->mNormals[i];

    auto _text = mesh->mTextureCoords[0];
    auto text = aiVector3D(0, 0, 0);

    if (_text)
      text = _text[i];

    vertices.push_back(ll::VertexC(
        glm::vec3(vert.x, vert.y, vert.z), glm::vec3(norm.x, norm.y, norm.z),
        glm::vec2(text.x, text.y), glm::vec4(0.2, 0.2, 0.2, 1)));
  }

  for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (unsigned int j = 0; j < face.mNumIndices; j++)
      indices.push_back(face.mIndices[j]);
  }

  ll::Texture tex;
  if (mesh->mMaterialIndex >= 0) {
    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
    tex = load_texture(material, aiTextureType_DIFFUSE, dir);
  }

  auto mat =
      pMaterial(new Material(shader, std::make_shared<ll::Texture>(tex)));

  return Mesh(mat, vertices, indices);
}

void ModelLoader::process_node(pShader shader, aiNode *node,
                               const aiScene *scene, const std::string &dir) {
  for (unsigned int i = 0; i < node->mNumMeshes; i++) {
    aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
    meshes.push_back(
        std::make_shared<Mesh>(process_mesh(shader, mesh, scene, dir)));
  }
  for (unsigned int i = 0; i < node->mNumChildren; i++) {
    process_node(shader, node->mChildren[i], scene, dir);
  }
}

Model ModelLoader::load(pShader shader, std::string path) {
  this->meshes.clear();
  Assimp::Importer importer;

  const aiScene *scene = importer.ReadFile(
      path, aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
      !scene->mRootNode) {
    std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
    importer.FreeScene();
    return Model();
  }
  std::string dir = path.substr(0, path.find_last_of('/')) + "/";
  process_node(shader, scene->mRootNode, scene, dir);
  importer.FreeScene();
  return Model(this->meshes);
}
} // namespace En
