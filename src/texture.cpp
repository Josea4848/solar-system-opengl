
#define STB_IMAGE_IMPLEMENTATION
#include "../include/texture.h"

GLuint loadTexture(const char *filename) {
  int width, height, nrChannels;
  unsigned char *data = stbi_load(filename, &width, &height, &nrChannels, 0);

  if (!data) {
    std::cerr << "Falha ao carregar textura: " << filename << std::endl;
    return 0;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);

  // Evitar problemas de alinhamento (largura não múltipla de 4 bytes)
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Parâmetros de wrap e filtro
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // Detecta RGB ou RGBA
  if (nrChannels == 3) {
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, width, height, GL_RGB,
                      GL_UNSIGNED_BYTE, data);
  } else if (nrChannels == 4) {
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, width, height, GL_RGBA,
                      GL_UNSIGNED_BYTE, data);
  } else {
    std::cerr << "Formato de imagem não suportado (nrChannels = " << nrChannels
              << ") em " << filename << std::endl;
    stbi_image_free(data);
    return 0;
  }

  stbi_image_free(data);
  return texture;
}
