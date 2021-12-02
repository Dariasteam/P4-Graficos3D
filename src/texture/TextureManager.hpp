#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "Texture.hpp"
#include "../aux/auxiliar.h"

#include <map>
#include <string>
#include <iostream>

class TextureManager {
private:
  GLfloat fLargest;
  std::map<std::string, Texture> textures;				  // Texture identifiers
  unsigned int loadTex(const char *fileName);
  TextureManager () {}
public:

  unsigned min_index_uniform_tex {0};

  inline static TextureManager& get () {
    static TextureManager instance;
    return instance;
  }

  TextureManager (const TextureManager&) = delete;
  void operator= (const TextureManager&) = delete;

  const Texture& get_texture (const std::string& name);
  bool load_texture(const std::string& path, const std::string& name);

  void prepare();
  void clean();
};

#endif // _TEXTURE_MANAGER_H_