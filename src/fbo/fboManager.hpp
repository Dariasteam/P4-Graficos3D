#ifndef FBO_MANAGER_H_
#define FBO_MANAGER_H_

#include "../texture/Texture.hpp"
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

  Texture color_fbo_tex;
  Texture specular_fbo_tex;
  Texture z_fbo_tex;
  Texture normal_fbo_tex;

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
    glBufferData(GL_ARRAY_BUFFER, planeNVertex * sizeof(float) * 3,
    planeVertexPos, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glGenFramebuffers(1, &fbo);

    glGenTextures(1, &colorBuffTexId);
    glGenTextures(1, &depthBuffTexId);
    glGenTextures(1, &zBuffTexId);
  }

  void resizeFBO(unsigned int w, unsigned int h) {

	glBindTexture(GL_TEXTURE_2D, colorBuffTexId);     // Activar la textura

	 // Dar formato y reservar uan textura MUTABLE pero no subimos información
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0,
							 GL_RGBA, GL_FLOAT, NULL);

	// Gestión de las situaciones de aliasing con los mipmaps
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // FIXME: Linear aquí o abajo?
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	// Imagen del z buffer usada en el postproceso (Coordenadas de la cámara)
	glBindTexture(GL_TEXTURE_2D, zBuffTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, w, h, 0,
						   GL_RGBA, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// Igual para textura del buffer Z
	glBindTexture(GL_TEXTURE_2D, depthBuffTexId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, w, h, 0,
							 GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1,
											 GL_TEXTURE_2D, zBuffTexId, 0);

	/*
		Asigna al color_attachment0 la textura de color, el 0 indica el
		nivel de mipmap sobre el que escribimos
	*/
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
											 	GL_TEXTURE_2D, colorBuffTexId, 0);

	// Esto está en coordenadas normalizadas del z buffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
												 depthBuffTexId, 0);



	/*
	const GLenum buffs[2] = {GL_NONE, GL_COLOR_ATTACHMENT0};
	glDrawBuffers(2, buffs);
	*/
	/*
	 	A la salida 1 del shader le vamos a asignar el color_attachment0.
		A la salida 0 del shader le vamos a asignar el color_attachment1
	*/
	const GLenum buffs[2] = {GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT0};
	glDrawBuffers(2, buffs);


	// Comprobar si el FBO está bien construido
	if (GL_FRAMEBUFFER_COMPLETE != glCheckFramebufferStatus(GL_FRAMEBUFFER))
	{
		std::cerr << "Error configurando el FBO" << std::endl;
		exit(-1);
	}

	// Activa el frambuffer por defecto? A cuenta de qué. Para dejar preparada la seugnda pasada?
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


  }

  void render () {

  }

};

#endif // FBO_MANAGER_H_