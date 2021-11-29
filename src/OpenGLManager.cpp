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
  scene_manager.get_current_scene()->clean();
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
void OGLManager::init_callbacks() {
  glutReshapeFunc(resizeFunc);
  glutDisplayFunc(renderFunc);
  glutIdleFunc(idleFunc);
  glutKeyboardFunc(keyboardFunc);
  glutMotionFunc(mouseMotionFunc);
}
void OGLManager::renderFunc() { scene_manager.get_current_scene()->render(); }

void OGLManager::mouseMotionFunc(int x, int y) {
  scene_manager.get_current_scene()->on_mouse_motion(x, y);
}
void OGLManager::mouseFunc(int button, int state, int x, int y) {
  scene_manager.get_current_scene()->on_mouse_button(button, state, x, y);
}
void OGLManager::keyboardFunc(unsigned char key, int x, int y) {
  scene_manager.get_current_scene()->on_keyboard(key);
}
void OGLManager::idleFunc() { scene_manager.get_current_scene()->on_idle(); }
void OGLManager::resizeFunc(int width, int height) {
  scene_manager.get_current_scene()->on_resize(width, height);
}
