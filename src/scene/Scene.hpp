#ifndef _SCENE_H_
#define _SCENE_H_

#include "../spatial/mesh/MeshLoader.h"
#include "../spatial/WorldManager.hpp"
#include "../shader/ShaderManager.hpp"
#include "../texture/TextureManager.hpp"
#include "../material/MaterialManager.hpp"
#include "../ogl/VBOManager.hpp"

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <functional>

struct Scene {
public:
  void clean () {}
  static Scene* generate_default ();
  static Scene* generate_scene_2 ();

  std::function<void (void)> init = [](void){};
  std::function<void (int width, int height)>  on_resize = [](int, int){};
  std::function<void (unsigned char key)>  on_keyboard = [](unsigned char){};
  std::function<void (int button, int state, int x, int y)> on_mouse_button  = [](int, int, int, int){};
  std::function<void (int x, int y)> on_mouse_motion  = [](int, int){};
  std::function<void (void)> on_idle = [](void){};;
  std::function<void (void)> render = [](void){};;
};

#endif //_SCENE_H_