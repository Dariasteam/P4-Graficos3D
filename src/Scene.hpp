#ifndef _SCENE_H_
#define _SCENE_H_

#include "ShaderManager.hpp"
#include "TextureManager.hpp"
#include "Camera.h"
#include "Material.hpp"
#include "Spatial.h"
#include "auxiliar.h"
#include "VBOHandler.hpp"
#include "ShaderManager.hpp"
#include "MeshLoader.h"

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <functional>

struct Scene {
public:
  double dummy_time = 0;

  AbstractCameraHandler* camera;
  std::vector<Spatial*> scene_objects;

  static ShaderManager shader_manager;
  static TextureManager texture_manager;
  static VBOHandler vbo_handler;
  static MeshLoader loader;

  void end () {
    // clean all handlers
  }

public:
  static Scene* generate_default ();

  std::function<void (const std::string& scene_name)> change_scene;
  std::function<void (void)> init;

  std::function<void (int width, int height)>  on_resize;
  std::function<void (unsigned char key)>  on_keyboard;
  std::function<void (int button, int state, int x, int y)> on_mouse_button;
  std::function<void (int x, int y)> on_mouse_motion;
  std::function<void (void)> on_idle;
  std::function<void (void)> render;

};

#endif //_SCENE_H_