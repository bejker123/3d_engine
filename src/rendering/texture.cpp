#include "texture.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "../stb/stb_image.h"

Texture::Texture(const char *fileName, GLenum type) {
  this->type = type;
  this->loadFromFile((char *)fileName);
}

Texture::~Texture() { glDeleteTextures(1, &this->id); }

inline GLuint Texture::getID() const { return this->id; }

void Texture::bind(const GLuint texture_unit) {
  glActiveTexture(GL_TEXTURE0 + texture_unit);
  glBindTexture(this->type, this->id);
}

void Texture::unbind() {
  glActiveTexture(0);
  glBindTexture(this->type, 0);
}

void Texture::loadFromFile(char *fileName) {
  if (this->id) {
    glDeleteTextures(1, &this->id);
  }

  int width, height, nrChannels;
  unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
  std::cout << width << " " << height << std::endl;

  glGenTextures(1, &this->id);
  glBindTexture(GL_TEXTURE_2D, this->id);

  glTexParameteri(this->type, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(this->type, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(this->type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

  if (data) {
    glTexImage2D(this->type, 0, GL_RGB, this->width, this->height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(this->type);
    std::cout << "Texture loaded: " << fileName << std::endl;
  } else {
    std::cout << "ERROR in:" << __FILE__
              << " culdn't load texture: " << fileName << std::endl;
  }

  glActiveTexture(0);
  glBindTexture(this->type, 0);
  free(data);
}
