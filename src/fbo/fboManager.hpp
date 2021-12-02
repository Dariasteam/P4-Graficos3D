#ifndef FBO_MANAGER_H_
#define FBO_MANAGER_H_

#include "../shader/ShaderManager.hpp"
#include "../aux/PLANE.h"

class FboManager {
private:

  FboManager () {}

public:
  unsigned fbo;

  unsigned planeVAO;
  unsigned planeVertexVBO;

  unsigned colorBuffTexId;
  unsigned depthBuffTexId;
  unsigned zBuffTexId;

  inline static FboManager& get () {
    static FboManager instance;
    return instance;
  }

  FboManager (const FboManager&) = delete;
  void operator= (const FboManager&) = delete;


  void init () {
    auto& shader_manager = ShaderManager::get();

    // COMPILING POST PROCESS SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/post_processing.vert", "p_v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/post_processing.frag", "p_f0")) exit(-1);

    // LINKING POST PROCESS PROGRAMS
    if (!shader_manager.create_program("p_p0", "p_v0", "p_f0", false)) exit(-1);

    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glGenBuffers(1, &planeVertexVBO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, planeNVertex*sizeof(float) * 3,
    planeVertexPos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenFramebuffers(1, &fbo);
    glGenTextures(1, &colorBuffTexId);
    glGenTextures(1, &depthBuffTexId);
  }

  void resizeFBO(unsigned int w, unsigned int h) {
    glBindTexture(GL_TEXTURE_2D, colorBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_FLOAT, NULL);

    // Manage mipmaps aliasing
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    // z-buffer texture used in post processing (camera space)
    glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    // z-buffer texture used in post processing (screen space)
    glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
                        GL_TEXTURE_2D, zBuffTexId, 0);

    // Assign color_attachment_0 to the texture colorBufferTexId (0 = mipmap)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
											 	GL_TEXTURE_2D, colorBuffTexId, 0);


    /*   A la salida 1 del shader le vamos a asignar el color_attachment0.
      A la salida 0 del shader le vamos a asignar el color_attachment1
      A la salida 1 del shader le vamos a asignar el color_attachment0.
      A la salida 0 del shader le vamos a asignar el color_attachment1
	  */
    const GLenum buffs[2] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT0};
    glDrawBuffers(2, buffs);



    // check FBO correctly built
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Error configurando el FBO" << std::endl;
      exit(-1);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  }

  void render () {

  }

};

#endif // FBO_MANAGER_H_