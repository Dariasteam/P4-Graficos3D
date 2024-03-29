#ifndef FBO_MANAGER_H_
#define FBO_MANAGER_H_

#include "../spatial/light/LightManager.hpp"
#include "../material/MaterialManager.hpp"
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
  unsigned deferred_fbo;
  unsigned post_processing_fbo;

  unsigned planeVAO;
  unsigned planeVertexVBO;

  Material mat_lightning_passes;
  Material mat_lightning_base;
  Material mat_post_processing;

  inline static FboManager& get () {
    static FboManager instance;
    return instance;
  }

  FboManager (const FboManager&) = delete;
  void operator= (const FboManager&) = delete;


  void init () {
    auto& shader_manager = ShaderManager::get();


    // COMPILING DEFERRED LIGHTNING SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/deferred_lightning_base.vert", "p_vbase")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/deferred_lightning_base.frag", "p_fbase")) exit(-1);

    if (!shader_manager.load_vertex_shader("shaders_P4/deferred_lightning_pass.vert", "p_v0")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/deferred_lightning_pass.frag", "p_f0")) exit(-1);

    // LINKING DEFERRED LIGHTNING SHADERS
    if (!shader_manager.create_program("p_p0", "p_v0", "p_f0")) exit(-1);
    if (!shader_manager.create_program("p_pbase", "p_vbase", "p_fbase")) exit(-1);



    // COMPILING POST PROCESS SHADERS
    if (!shader_manager.load_vertex_shader("shaders_P4/post_processing.vert", "post_processing.vert")) exit(-1);
    if (!shader_manager.load_fragment_shader("shaders_P4/post_processing.frag", "post_processing.frag")) exit(-1);

    // LINKING POST PROCESS SHADERS
    if (!shader_manager.create_program("post_processing",
                                       "post_processing.vert",
                                       "post_processing.frag")) exit(-1);


    mat_lightning_passes = MaterialManager::get().create_material();
    mat_post_processing = MaterialManager::get().create_material();
    mat_lightning_base = MaterialManager::get().create_material();

    glGenVertexArrays(1, &planeVAO);
    glBindVertexArray(planeVAO);
    glGenBuffers(1, &planeVertexVBO);

    glBindBuffer(GL_ARRAY_BUFFER, planeVertexVBO);
    glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float) * 3,
    planeVertexPos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenFramebuffers(1, &deferred_fbo);
    glGenFramebuffers(1, &post_processing_fbo);

    auto& texture_manager = TextureManager::get();

    texture_manager.generate_empty("color_fbo");
    texture_manager.generate_empty("normal_fbo");
    texture_manager.generate_empty("specular_fbo");
    texture_manager.generate_empty("depth_fbo");
    texture_manager.generate_empty("emissive_fbo");
    texture_manager.generate_empty("z_fbo");
    texture_manager.generate_empty("pos_fbo");
    texture_manager.generate_empty("post_process_fbo");

    mat_lightning_passes.shader_mat_uniforms["zTex"] = new SP_Texture(texture_manager.get_texture("z_fbo"));
    mat_lightning_passes.shader_mat_uniforms["depthTex"] = new SP_Texture(texture_manager.get_texture("depth_fbo"));
    mat_lightning_passes.shader_mat_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("color_fbo"));
    mat_lightning_passes.shader_mat_uniforms["normalTex"] = new SP_Texture(texture_manager.get_texture("normal_fbo"));
    mat_lightning_passes.shader_mat_uniforms["specularTex"] = new SP_Texture(texture_manager.get_texture("specular_fbo"));
    mat_lightning_passes.shader_mat_uniforms["positionTex"] = new SP_Texture(texture_manager.get_texture("pos_fbo"));


    mat_lightning_base.shader_mat_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("color_fbo"));
    mat_lightning_base.shader_mat_uniforms["zTex"] = new SP_Texture(texture_manager.get_texture("z_fbo"));
    mat_lightning_base.shader_mat_uniforms["specularTex"] = new SP_Texture(texture_manager.get_texture("specular_fbo"));
    mat_lightning_base.shader_mat_uniforms["emiTex"] = new SP_Texture(texture_manager.get_texture("emissive_fbo"));
    mat_lightning_base.shader_mat_uniforms["normalTex"] = new SP_Texture(texture_manager.get_texture("normal_fbo"));
    mat_lightning_base.shader_mat_uniforms["positionTex"] = new SP_Texture(texture_manager.get_texture("pos_fbo"));

    mat_post_processing.shader_mat_uniforms["zTex"] = new SP_Texture(texture_manager.get_texture("z_fbo"));
    mat_post_processing.shader_mat_uniforms["colorTex"] = new SP_Texture(texture_manager.get_texture("post_process_fbo"));

    shader_manager.bind_material("p_p0", mat_lightning_passes);
    shader_manager.bind_material("p_pbase", mat_lightning_base);
    shader_manager.bind_material("post_processing", mat_post_processing);


    // BIND LIGHT IDS IN PROGRAM TO LIGHTS
    // single pass lights
    LightManager::get().bind_ambient_light("p_pbase");
    LightManager::get().bind_directional_light("p_pbase");

    // multiple pass lights
    LightManager::get().bind_focal_light("p_p0");
    LightManager::get().bind_point_light("p_p0");
  }

  void generate_post_processing_tex (unsigned w, unsigned h) {
    const Texture& t = TextureManager::get().get_texture("post_process_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT5,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_color_tex (unsigned w, unsigned h) {
    const Texture& t = TextureManager::get().get_texture("color_fbo");

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
    const Texture& t = TextureManager::get().get_texture("normal_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0,
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
    const Texture& t = TextureManager::get().get_texture("specular_fbo");

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

  void generate_emissive_tex (unsigned w, unsigned h) {
    const Texture& t = TextureManager::get().get_texture("emissive_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    // Dar formato y reservar uan textura MUTABLE pero no subimos información
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    // Gestión de las situaciones de aliasing con los mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT6,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_pos_tex (unsigned w, unsigned h) {
    const Texture& t = TextureManager::get().get_texture("pos_fbo");

    glBindTexture(GL_TEXTURE_2D, t.id);     // Activar la textura
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, w, h, 0,
                GL_RGBA, GL_FLOAT, NULL);

    // Gestión de las situaciones de aliasing con los mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4,
											 	GL_TEXTURE_2D, t.id, 0);
  }

  void generate_depth_tex (unsigned w, unsigned h) {
    const Texture& t = TextureManager::get().get_texture("depth_fbo");

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
    const Texture& t = TextureManager::get().get_texture("z_fbo");
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
    resize_deferred_fbo (w, h);
    resize_post_processing_fbo(w, h);
  }

  void resize_deferred_fbo(unsigned int w, unsigned int h) {

    glBindFramebuffer(GL_FRAMEBUFFER, deferred_fbo);

    generate_color_tex(w, h);
    generate_normal_tex(w, h);
    generate_specular_tex(w, h);
    generate_depth_tex(w, h);
    generate_z_tex(w, h);
    generate_pos_tex (w, h);
    generate_emissive_tex(w, h);

    const GLenum buffs_deferred[6] = {GL_COLOR_ATTACHMENT1,
                                      GL_COLOR_ATTACHMENT0,
                                      GL_COLOR_ATTACHMENT2,
                                      GL_COLOR_ATTACHMENT3,
                                      GL_COLOR_ATTACHMENT6,
                                      GL_COLOR_ATTACHMENT4,
                                      };


    glDrawBuffers(6, buffs_deferred);

    // Comprobar si el FBO está bien construido
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Error configurando el FBO de deferred" << std::endl;
      exit(-1);
    }
  }

  void resize_post_processing_fbo(unsigned int w, unsigned int h) {

    glBindFramebuffer(GL_FRAMEBUFFER, post_processing_fbo);

    generate_post_processing_tex(w, h);

	  const GLenum buffs_post_processing[2] = {GL_COLOR_ATTACHMENT5,
                                             GL_COLOR_ATTACHMENT0};

    glDrawBuffers(2, buffs_post_processing);

    // Comprobar si el FBO está bien construido
    if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
    {
      std::cerr << "Error configurando el FBO de post procesado" << std::endl;
      exit(-1);
    }
  }

  void render () {

  }

};

#endif // FBO_MANAGER_H_