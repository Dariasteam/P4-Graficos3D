#include "OpenGLManager.h"
#include "SceneManager.hpp"

void OGLManager::init_OGL () {
  glEnable(GL_DEPTH_TEST);
	glClearColor(0.2f, 0.2f, 0.2f, 0.0f);

	glFrontFace(GL_CCW);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
}

void OGLManager::destroy() {
  /*

  for (const auto &p : programs)
    p.second->detach();

  for (auto s : vertex_shaders)
    delete s.second;

  for (auto s : fragment_shaders)
    delete s.second;

  for (const auto &p : programs)
    delete p.second;
  */
}
void OGLManager::init_context(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitContextVersion(3, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(500, 500);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Prácticas OGL");

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err) {
    std::cout << "Error: " << glewGetErrorString(err) << std::endl;
    exit(-1);
  }
  const GLubyte *oglVersion = glGetString(GL_VERSION);
  std::cout << "This system supports OpenGL Version: " << oglVersion
            << std::endl;
}

void OGLManager::start_loop() {
  glutMainLoop();
}

SceneManager OGLManager::scene_manager;