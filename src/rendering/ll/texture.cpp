#include "texture.hpp"
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "../../logger.hpp"
#include "../../stb/stb_image.h"

static std::map<std::string, uint32_t> hashes;

Texture::Texture(std::string file, GLenum type) {
  this->type = type;
  this->load_from_file(file);
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }

GLuint Texture::get_id() const { return this->id; }

void Texture::bind(const GLuint texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(this->type, this->id);
}

void Texture::unbind(const uint32_t type) {
  // Fix debug info:
  //  glActiveTexture(0);
  glBindTexture(type, 0);
  // glDisable(type);
}
void Texture::load_from_file(std::string file) {
  if (this->id) {
    glDeleteTextures(1, &this->id);
  }

  glGenTextures(1, &this->id);
  glBindTexture(this->type, this->id);

  auto idx = hashes.end();
  if ((idx = hashes.find(file)) != hashes.end()) {
    LOG("Texture Loaded Before: %s\n\t Using Previous.\n", file.c_str());
    this->id = idx->second;
    glBindTexture(this->type, 0);
    return;
  } else
    hashes.insert(std::make_pair(file, this->id));

  // std::cout << x << std::endl;

  stbi_set_flip_vertically_on_load(true);
  int nrChannels;
  unsigned char *data =
      stbi_load(file.data(), &this->width, &this->height, &nrChannels, 0);
  // std::cout << this->width << " " << this->height << std::endl;

  glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (data != nullptr) {
    glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(this->type);
    // std::cout << "Texture loaded: " << file << std::endl;
    LOG("Texture Loaded, Debug Info:\n\tSize: (%d,%d)\n\tID: %d\n", this->width,
        this->height, this->id);
  } else {
    LOG("Failed to load texture: %s", file.c_str());
    // std::cout << "ERROR in:" << __FILE__ << " culdn't load texture: " << file
    //           << std::endl;
  }

  // glActiveTexture(0);
  glBindTexture(this->type, 0);
  stbi_image_free(data);
}
