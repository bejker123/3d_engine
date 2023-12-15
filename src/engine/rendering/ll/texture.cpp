#include "texture.hpp"
#include <cstdint>
#include <map>
#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb/stb_image.h"
#include "../../io/logger.hpp"

namespace En {
namespace ll {

static std::map<std::string, uint32_t> hashes;

Texture::Texture(const std::string file, const bool transparent,
                 const GLenum type)
    : type(type), transparent(transparent) {
  this->load_from_file(file);
}

void Texture::terminate() const {
  hashes.clear();
  glDeleteTextures(1, &this->id);
}

GLuint ll::Texture::get_id() const { return this->id; }

void ll::Texture::bind(const GLuint texture_unit) const {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(this->type, this->id);
}

void ll::Texture::unbind(const uint32_t type) { glBindTexture(type, 0); }
void ll::Texture::load_from_file(std::string file) {
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

  // std::cout << x << std::endl;

  stbi_set_flip_vertically_on_load(true);
  int nrChannels;
  unsigned char *data =
      stbi_load(file.data(), &this->width, &this->height, &nrChannels, 0);

  glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (data != nullptr) {
    GLenum format = GL_RGB;
    if (nrChannels == 1)
      format = GL_RED;
    else if (nrChannels == 4)
      format = GL_RGBA;

    GLenum format1 = this->transparent && nrChannels == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(this->type, 0, format1, this->width, this->height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(this->type);
    LOG("Texture Loaded, Debug Info:\n\tSize: (%d,%d)\n\tID: %d\n\t# Channels: "
        "%d"
        "\n",
        this->width, this->height, this->id, nrChannels);
  } else {
    LOG("Failed to load texture: %s\n", file.c_str());
  }

  glBindTexture(this->type, 0);
  stbi_image_free(data);
}

} // namespace ll
} // namespace En
