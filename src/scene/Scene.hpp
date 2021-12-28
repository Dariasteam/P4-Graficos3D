#ifndef _SCENE_H_
#define _SCENE_H_

#include "../fbo/fboManager.hpp"
#include "../spatial/light/LightManager.hpp"
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

  Scene (const std::function<void (Scene& scene)>& data) {
    data (*this);
  }

  std::function<void (void)> init = [](void) {
    std::cerr << "Warning: Llamada a 'init' sin implementar" <<std::endl;
  };

  std::function<void (int width, int height)>  on_resize = [](int, int) {
    std::cerr << "Warning: Llamada a 'on_resize' sin implementar" <<std::endl;
  };

  std::function<void (unsigned char key)>  on_keyboard = [](unsigned char) {
    std::cerr << "Warning: Llamada a 'on_keyboard' sin implementar" <<std::endl;
  };

  std::function<void (int button, int state, int x, int y)> on_mouse_button = [](int, int, int, int) {
    std::cerr << "Warning: Llamada a 'on_mouse_button' sin implementar" <<std::endl;
  };

  std::function<void (int x, int y)> on_mouse_motion = [](int, int) {
    std::cerr << "Warning: Llamada a 'on_mouse_motion' sin implementar" <<std::endl;
  };

  std::function<void (void)> on_idle = [](void){
    std::cerr << "Warning: Llamada a 'on_idle' sin implementar" <<std::endl;
  };

  std::function<void (void)> render = [](void) {
    std::cerr << "Warning: Llamada a 'render' sin implementar" <<std::endl;
  };
};

#endif //_SCENE_H_