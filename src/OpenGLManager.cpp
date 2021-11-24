#include "OpenGLManager.h"
#include <vector>

// Guarantees that both texture id and uniform Id are created
bool OpenGLManager::load_texture(const std::string& path,
                                 const std::string& name) {
  int tmp_u_tex;
  int tmp_tex_id;

  tmp_tex_id = loadTex(path.c_str());

  if (tmp_tex_id == -1) return false;

  glUniform1i(tmp_u_tex, tmp_tex_id);

  if (tmp_u_tex == -1) return false;

  texture_ids[name] = tmp_tex_id;
  texture_unit_handler[name] = tmp_u_tex;

  return true;
}

unsigned int OpenGLManager::loadTex(const char *fileName) {
  unsigned char *map;
  unsigned int w, h;
  map = loadTexture(fileName, w, h);

  if (!map) {
    std::cout << "Error cargando el fichero: " << fileName << std::endl;
    return -1;
  }

  unsigned int texId;
  glGenTextures(1, &texId);
  glBindTexture(GL_TEXTURE_2D, texId);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
               (GLvoid *)map);

  delete[] map;

  glGenerateMipmap(GL_TEXTURE_2D);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

  return texId;
}


int OpenGLManager::boundProgramParameters (Program& program) {

  // We are using indexes because the shaders are using layout / location

  if (program.attributes["inPos"] != -1)
    glEnableVertexAttribArray(0);

  if (program.attributes["inColor"] != -1)
    glEnableVertexAttribArray(1);

  if (program.attributes["inNormal"] != -1)
    glEnableVertexAttribArray(2);

  if (program.attributes["inTexCoord"] != -1)
    glEnableVertexAttribArray(3);
}


int OpenGLManager::instantiateMesh(const unsigned n_vertices,
                                   const unsigned n_faces,
                                   const unsigned *faceIndices,
                                   const float *vertexCoord,
                                   const float *vertexColors,
                                   const float *normals, const float *texCoords,
                                   const float *tangents,
                                   const Program& program) {

  glGenBuffers(1, &posVBO);
  glGenBuffers(1, &colorVBO);
  glGenBuffers(1, &normalVBO);
  glGenBuffers(1, &texCoordVBO);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);


  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 2, texCoords,
               GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, posVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, vertexCoord,
               GL_STATIC_DRAW);


  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, colorVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, vertexColors,
               GL_STATIC_DRAW);


  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
  glBufferData(GL_ARRAY_BUFFER, n_vertices * sizeof(float) * 3, normals,
               GL_STATIC_DRAW);


  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, texCoordVBO);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);


  glGenBuffers(1, &triangleIndexVBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleIndexVBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, n_faces * sizeof(unsigned int) * 3,
               faceIndices, GL_STATIC_DRAW);
}

bool OpenGLManager::load_vertex_shader (const std::string& path,
                                        const std::string& name) {
  int vshader = loadShader(path.c_str(), GL_VERTEX_SHADER);

  if (vshader < 0) return false;

  VertexShader* V = new VertexShader;
  V->id = vshader;
  vertex_shaders[name] = V;

  return true;
}

bool OpenGLManager::load_fragment_shader (const std::string& path,
                                          const std::string& name) {

  int fshader = loadShader(path.c_str(), GL_FRAGMENT_SHADER_ARB);

  if (fshader < 0) return false;

  FragmentShader* F = new FragmentShader;
  F->id = fshader;
  fragment_shaders[name] = F;

  return true;
}

bool OpenGLManager::create_program(const std::string& program_name,
                                   const VertexShader& vertex_shader,
											             const FragmentShader& fragment_shader,
                                   const std::vector<std::string>& uniforms_names,
                                   const std::vector<std::string>& attributes_names,
                                   int pos) {

  const unsigned program = glCreateProgram();

  unsigned V = vertex_shader.id;
  unsigned F = fragment_shader.id;

  Program* aux_program = new Program;
  aux_program->id = program;

  aux_program->vertex = &vertex_shader;
  aux_program->fragment = &fragment_shader;

	glAttachShader(program, V);
	glAttachShader(program, F);
	glLinkProgram(program);

  int linked;

	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program, logLen, NULL, logString);
		std::cerr << "Error linking program: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program);

    delete aux_program;

		return false;
	}

  // Uniforms IDs
  for (const std::string& name : uniforms_names) {
    int aux = glGetUniformLocation(program, name.c_str());
    if (aux < 0) {
      std::cout << "Error creando el uniform " << name << " " << aux << " para el programa " << program << std::endl;
    }
    aux_program->uniforms[name] = aux;
  }

  // Attributes IDs
  for (const std::string& name : attributes_names) {
    int aux = glGetAttribLocation(program, name.c_str());
    if (aux < 0) {
      std::cout << "Error creando el atributo " << name << " " << aux << " para el programa " << program << std::endl;
    }
    aux_program->attributes[name] = aux;
  }

  programs[program_name] = aux_program;
  return true;
}

void OpenGLManager::set_mesh_per_program (Program& program,
                                          MeshInstance* mesh) const {

  program.asociated_meshes.insert(mesh);
}

GLuint OpenGLManager::loadShader(const char *fileName, GLenum type) {
  unsigned int fileLen;
  char *source = loadStringFromFile(fileName, fileLen);

  //////////////////////////////////////////////
  // Creación y compilación del Shader
  GLuint shader;
  shader = glCreateShader(type);
  glShaderSource(shader, 1, (const GLchar **)&source, (const GLint *)&fileLen);
  glCompileShader(shader);
  delete[] source;

  GLint compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    // Calculamos una cadena de error
    GLint logLen;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
    char *logString = new char[logLen];
    glGetShaderInfoLog(shader, logLen, NULL, logString);
    std::cout << "Error: " << logString << std::endl;
    delete[] logString;
    glDeleteShader(shader);
    //exit(-1);
    return -1;
  }

  return shader;
}

void OpenGLManager::init_context() {

}

void OpenGLManager::init_OGL() {}


void OpenGLManager::destroy() {
  glDeleteBuffers(1, &posVBO);
  glDeleteBuffers(1, &colorVBO);
  glDeleteBuffers(1, &normalVBO);
  glDeleteBuffers(1, &texCoordVBO);
  glDeleteBuffers(1, &triangleIndexVBO);
  glDeleteVertexArrays(1, &vao);

  for (const auto &p : programs) {
    const auto program = *p.second;
    glDetachShader(program.id, program.vertex->id);
    glDetachShader(program.id, program.fragment->id);
  }

  for (const auto& s : vertex_shaders)
    glDeleteShader(s.second->id);

  for (const auto& s : fragment_shaders)
    glDeleteShader(s.second->id);

  for (const auto &p : programs) {
    const auto program = *p.second;
    glDeleteProgram(program.id);
  }
}
