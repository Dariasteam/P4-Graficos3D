#include "OpenGLManager.h"
#include <vector>

// Guarantees that both texture id and uniform Id are created
bool OGLManager::load_texture(const std::string& path,
                              const std::string& name) {

  int tmp_tex_id = loadTex(path.c_str());

  if (tmp_tex_id == -1) return false;

  Texture aux_tex;
  aux_tex.id = tmp_tex_id;
  aux_tex.n_texture = texture_ids.size();

  texture_ids[name] = aux_tex;

  return true;
}

unsigned int OGLManager::loadTex(const char *fileName) {
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


int OGLManager::bound_program_attributes (Program& program,
                                          const std::map<std::string, unsigned>&
                                          attribute_name_location) {

  // We are using indexes because the shaders are using layout / location
  for (const auto& element : attribute_name_location) {
    if (program.attributes[element.first] != -1)
      glEnableVertexAttribArray(element.second);
  }
}

bool OGLManager::load_vertex_shader (const std::string& path,
                                     const std::string& name) {

  int vshader = loadShader(path.c_str(), GL_VERTEX_SHADER);

  if (vshader < 0) return false;

  VertexShader* V = new VertexShader;
  V->id = vshader;
  vertex_shaders[name] = V;

  return true;
}

bool OGLManager::load_fragment_shader (const std::string& path,
                                       const std::string& name) {

  int fshader = loadShader(path.c_str(), GL_FRAGMENT_SHADER_ARB);

  if (fshader < 0) return false;

  FragmentShader* F = new FragmentShader;
  F->id = fshader;
  fragment_shaders[name] = F;

  return true;
}

bool OGLManager::create_program(const std::string& program_name,
                                const VertexShader& vertex_shader,
											          const FragmentShader& fragment_shader,
                                const std::vector<std::string>& uniforms_names,
                                const std::vector<std::string>& attributes_names) {

  const unsigned program_id = glCreateProgram();

  unsigned V = vertex_shader.id;
  unsigned F = fragment_shader.id;

  Program* aux_program = new Program;
  aux_program->id = program_id;

  aux_program->vertex = &vertex_shader;
  aux_program->fragment = &fragment_shader;

	glAttachShader(program_id, V);
	glAttachShader(program_id, F);
	glLinkProgram(program_id);

  int linked;

	glGetProgramiv(program_id, GL_LINK_STATUS, &linked);

	if (!linked) {
		//Calculamos una cadena de error
		GLint logLen;
		glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &logLen);
		char* logString = new char[logLen];
		glGetProgramInfoLog(program_id, logLen, NULL, logString);
		std::cerr << "Error linking program: " << logString << std::endl;
		delete[] logString;
		glDeleteProgram(program_id);

    delete aux_program;

		return false;
	}


  // Uniforms IDs
  for (const std::string& name : uniforms_names) {
    int aux = glGetUniformLocation(program_id, name.c_str());
    if (aux < 0) {
      std::cerr << "Error creando el uniform " << name << " " << aux << " para el programa " << program_name << std::endl;
    }
    aux_program->uniforms[name] = aux;
  }

  // Unnecessary since we are using layout / location
/*
  // Attributes IDs
  for (const std::string& name : attributes_names) {
    int aux = glGetAttribLocation(program_id, name.c_str());
    if (aux < 0) {
      std::cerr << "Error creando el atributo " << name << " " << aux << " para el programa " << program_name << std::endl;
    }
    aux_program->attributes[name] = aux;
  }
*/

  programs[program_name] = aux_program;
  return true;
}

void OGLManager::set_mesh_per_program (Program& program,
                                       MeshInstance* mesh) const {

  program.associated_meshes.insert(mesh);
}

GLuint OGLManager::loadShader(const char *fileName, GLenum type) {
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
    return -1;
  }

  return shader;
}

void OGLManager::init_context() {

}

void OGLManager::init_OGL() {

}


void OGLManager::destroy() {

  for (const auto &p : programs)
    p.second->detach();

  for (auto s : vertex_shaders)
    delete s.second;

  for (auto s : fragment_shaders)
    delete s.second;

  for (const auto &p : programs)
    delete p.second;

  // FIXME: Release resources for textures
  for (const auto it : texture_ids) {
    //glDeleteTextures(1, it.second);
  }

}
