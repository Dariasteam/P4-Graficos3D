#include "TextureManager.hpp"

void TextureManager::generate_empty(const std::string& name) {

  unsigned tmp_tex_id;
  glGenTextures(1, &tmp_tex_id);

  Texture aux_tex;
  aux_tex.id = tmp_tex_id;
  aux_tex.n_texture = textures.size();

  textures[name] = aux_tex;
}

// Guarantees that both texture id and uniform id are created
bool TextureManager::load_texture(const std::string& path,
                                  const std::string& name) {

  int tmp_tex_id = loadTex(path.c_str());

  if (tmp_tex_id == -1) return false;

  Texture aux_tex;
  aux_tex.id = tmp_tex_id;
  aux_tex.n_texture = textures.size();

  textures[name] = aux_tex;

  return true;
}

unsigned TextureManager::loadTex(const char *fileName) {
  unsigned char *map;
  unsigned int w, h;
  map = loadTexture(fileName, w, h);

  if (!map) {
    std::cout << "Error cargando el fichero: " << fileName << std::endl;
    return -1;
  }

  unsigned int texId;

  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               (GLvoid *)map);

  delete[] map;

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, fLargest);
  return texId;
}

const Texture& TextureManager::get_texture (const std::string& name) {
  return textures[name];
}

void TextureManager::prepare() {
  if (glutExtensionSupported ("GL_EXT_texture_filter_anisotropic") != 0) {
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
    std::cout << "Max anisotropy level: " << fLargest << std::endl;
	} else {
    std::cout << "This system does not support anisotropy" << std::endl;
  }
}

void TextureManager::clean() {
  for (auto& texture : textures)
    glDeleteTextures (1, &texture.second.id);

  textures.clear();
}
