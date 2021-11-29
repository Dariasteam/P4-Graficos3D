#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "Texture.hpp"
#include "auxiliar.h"

#include <map>
#include <string>
#include <iostream>

class TextureManager {
private:
  GLfloat fLargest;
  std::map<std::string, Texture> textures;				  // Texture identifiers
  unsigned int loadTex(const char *fileName);
public:
  const Texture& get_texture (const std::string& name);
  bool load_texture(const std::string& path, const std::string& name);
  void prepare();
  TextureManager ();
};

#endif // _TEXTURE_MANAGER_H_