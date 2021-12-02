#ifndef FBO_MANAGER_H_
#define FBO_MANAGER_H_

#include "../texture/TextureManager.hpp"
#include "../shader/ShaderManager.hpp"
#include "../aux/PLANE.h"

struct FboTexture : public Texture{
  unsigned uniform_id;
};

class FboManager {
private:

  FboManager () {}

public:
  unsigned fbo;

  unsigned planeVAO;
  unsigned planeVertexVBO;

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
    if (!shader_manager.create_program("p_p0", "p_v0", "p_f0", ShaderManager::P_PROJECTION)) exit(-1);

    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glGenBuffers(1, &planeVertexVBO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float) * 3,
    planeVertexPos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenFramebuffers(1, &fbo);

    auto& texture_manager = TextureManager::get();

    texture_manager.generate_empty("color_fbo");
    texture_manager.generate_empty("normal_fbo");
    texture_manager.generate_empty("specular_fbo");
    texture_manager.generate_empty("depth_fbo");
    texture_manager.generate_empty("z_fbo");
  }

  void generate_color_tex (unsigned w, unsigned h) {
    Texture t = TextureManager::get().get_texture("color_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    // Dar formato y reservar uan textura MUTABLE pero no subimos información
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    // Gestión de las situaciones de aliasing con los mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_normal_tex (unsigned w, unsigned h) {
    Texture t = TextureManager::get().get_texture("normal_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    // Gestión de las situaciones de aliasing con los mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_specular_tex (unsigned w, unsigned h) {
    Texture t = TextureManager::get().get_texture("specular_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    // Gestión de las situaciones de aliasing con los mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_depth_tex (unsigned w, unsigned h) {
    Texture t = TextureManager::get().get_texture("depth_fbo");

    // Imagen del z buffer usada en el postproceso (Coordenadas normalizadas Z)
    glBindTexture(GL_TEXTURE_2D, t.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
                GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
												   t.id, 0);
  }

  void generate_z_tex (unsigned w, unsigned h) {
    Texture t = TextureManager::get().get_texture("z_fbo");
    // Imagen del z buffer usada en el postproceso (Coordenadas de la cámara)
    glBindTexture(GL_TEXTURE_2D, t.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
											     GL_TEXTURE_2D, t.id, 0);
  }


  void resizeFBO(unsigned int w, unsigned int h) {
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    generate_color_tex(w, h);
    generate_normal_tex(w, h);
    generate_specular_tex(w, h);
    generate_depth_tex(w, h);
    generate_z_tex(w, h);

	  const GLenum buffs[2] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT0};
	  glDrawBuffers(2, buffs);

    // Comprobar si el FBO está bien construido
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Error configurando el FBO" << std::endl;
      exit(-1);
    }
  }

  void render () {

  }

};

#endif // FBO_MANAGER_H_