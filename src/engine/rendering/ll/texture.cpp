#include "texture.hpp"
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb/stb_image.h"
#include "../../io/logger.hpp"

namespace En {

static std::map<std::string, uint32_t> hashes;

Texture::Texture(const std::string file, const bool transparent,
                 const GLenum type)
    : type(type), transparent(transparent) {
  this->load_from_file(file);
}

Texture::Texture(std::vector<std::string> faces)
    : type(GL_TEXTURE_CUBE_MAP), transparent(false) {
  this->load_cubemap(faces);
}

void Texture::terminate() const {
  hashes.clear();
  glDeleteTextures(1, &this->id);
}

GLuint Texture::get_id() const { return this->id; }

void Texture::bind(const GLuint texture_unit) const {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(this->type, this->id);
}

void Texture::unbind(const uint32_t type) { glBindTexture(type, 0); }

void Texture::load_from_file(std::string file) {
  if (this->id != 0) {
    glDeleteTextures(1, &this->id);
  }
  glGenTextures(1, &this->id);

  auto idx = hashes.end();
  if ((idx = hashes.find(file)) != hashes.end()) {
    this->id = idx->second;
    glBindTexture(this->type, 0);
    return;
  } else
    hashes.insert(std::make_pair(file, this->id));
  glBindTexture(this->type, this->id);

  LOG("[TEXTRURE] LOADING: %s\n", file.data());
  stbi_set_flip_vertically_on_load(true);
  int nr_channels;
  unsigned char *data =
      stbi_load(file.data(), &this->width, &this->height, &nr_channels, 0);

  if (data != nullptr) {
    GLenum format = GL_RGB;
    if (nr_channels == 1)
      format = GL_RED;
    else if (nr_channels == 4)
      format = GL_RGBA;

    GLenum format1 = this->transparent && nr_channels == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(this->type, 0, format1, this->width, this->height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(this->type);
    LOG("[TEXTURE] LOADED, Debug Info:\n\tSize: (%d,%d)\n\tID: %d\n\t# "
        "Channels: "
        "%d"
        "\n",
        this->width, this->height, this->id, nr_channels);
  } else {
    LOG("[TEXTURE] FAILED to load: %s\n", file.c_str());
  }

  glBindTexture(this->type, 0);
  stbi_image_free(data);
}

void Texture::load_cubemap(std::vector<std::string> faces) {
  glGenTextures(1, &id);
  glBindTexture(GL_TEXTURE_CUBE_MAP, id);

  int nr_channels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char *data =
        stbi_load(faces[i].c_str(), &width, &height, &nr_channels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      LOG("Cubemap tex failed to load at path: %s\n", faces[i].c_str());
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
  // Texture::unbind(GL_TEXTURE_CUBE_MAP);
}
} // namespace En
